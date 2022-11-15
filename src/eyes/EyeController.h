#pragma once

#include <Arduino.h>
#include <vector>
#include <memory>
#include <cmath>
#include "eyes.h"

/// Manages the overall behaviour (movement, blinking, pupil size) of one or more eyes.
template<std::size_t numEyes, typename Disp>
class EyeController {
private:
  std::array<Eye<Disp>, numEyes> eyes{};

  uint32_t eyeIndex{};

  /// Holds the current overall state of the eye(s)
  OverallState state{};

  /// Whether the eyes are moving autonomously
  bool autoMove{};

  /// Whether the eyes are blinking autonomously
  bool autoBlink{};

  /// Whether the pupils are resizing autonomously
  bool autoPupils{};

  /// When autoMove is enabled, this represents the maximum amount of time the eyes
  /// gaze at a particular place before moving again.
  uint32_t maxGazeMs{3000};

  /// The amount of fixation to apply
  int nufix{7};

  // For autonomous iris scaling
  static constexpr size_t irisLevels{7};
  std::array<float, irisLevels> irisPrev{};
  std::array<float, irisLevels> irisNext{};

  /// If autoMove is enabled, periodically initiates motion to a new random point
  /// with a random speed, and holds there for random period until next motion.
  void applyAutoMove(Eye<Disp> &eye) {
    if (!autoMove && !state.inMotion) {
      eye.x = state.eyeOldX;
      eye.y = state.eyeOldY;
      return;
    }

    uint32_t t = millis();

    // milliseconds elapsed since last eye event
    uint32_t dt = t - state.moveStartTimeMs;

    if (state.inMotion) {
      // The eye is currently moving
      if (dt >= state.moveDurationMs) {
        // Time's up, we have reached the destination
        state.inMotion = false;
        // The "move" duration temporarily becomes a hold duration...
        // Normally this is 35 ms to 1 sec, but don't exceed gazeMax setting
        uint32_t limit = min(1000u, maxGazeMs);
        state.moveDurationMs = random(35u, limit);         // Time between micro-saccades
        if (!state.saccadeIntervalMs) {                    // Cleared when "big" saccade finishes
          state.lastSaccadeStopMs = t;                     // Time when saccade stopped
          state.saccadeIntervalMs = random(state.moveDurationMs, maxGazeMs); // Next in 30ms to 3sec
        }
        // Similarly, the "move" start time becomes the "stop" starting time...
        state.moveStartTimeMs = t;               // Save time of event
        eye.x = state.eyeOldX = state.eyeNewX;           // Save position
        eye.y = state.eyeOldY = state.eyeNewY;
      } else { // Move time's not yet fully elapsed -- interpolate position
        float e = (float) dt / float(state.moveDurationMs); // 0.0 to 1.0 during move
        float e2 = e * e;
        e = e2 * (3 - 2 * e);     // Easing function: 3*e^2-2*e^3, values in range 0.0 to 1.0
        eye.x = state.eyeOldX + (state.eyeNewX - state.eyeOldX) * e; // Interp X
        eye.y = state.eyeOldY + (state.eyeNewY - state.eyeOldY) * e; // and Y
      }
    } else {
      // Eye is currently stopped
      eye.x = state.eyeOldX;
      eye.y = state.eyeOldY;
      if (dt > state.moveDurationMs) {
        // It's time to begin a new move
        if ((t - state.lastSaccadeStopMs) > state.saccadeIntervalMs) {
          // It's time for a 'big' saccade. r is the radius in X and Y that the eye can go, from (0,0) in the center.
          float r = ((float) eye.definition->polar.mapRadius * 2 - (float) screenWidth * M_PI_2) * 0.75f;
          state.eyeNewX = random(-r, r);
          float moveDist = sqrtf(r * r - state.eyeNewX * state.eyeNewX);
          state.eyeNewY = random(-moveDist, moveDist);
          // Set the duration for this move, and start it going.
          state.moveDurationMs = random(83, 166); // ~1/12 - ~1/6 sec
          state.saccadeIntervalMs = 0; // Calc next interval when this one stops
        } else {
          // Microsaccade
          // r is possible radius of motion, ~1/10 size of full saccade.
          // We don't bother with clipping because if it strays just a little,
          // that's okay, it'll get put in-bounds on next full saccade.
          float r = (float) eye.definition->polar.mapRadius * 2 - (float) screenWidth * M_PI_2;
          r *= 0.07f;
          float dx = random(-r, r);
          state.eyeNewX = eye.x - eye.definition->polar.mapRadius + dx;
          float h = sqrtf(r * r - dx * dx);
          state.eyeNewY = eye.y - eye.definition->polar.mapRadius + random(-h, h);
          state.moveDurationMs = random(7, 25); // 7-25 ms microsaccade
        }
        state.eyeNewX += eye.definition->polar.mapRadius;    // Translate new point into map space
        state.eyeNewY += eye.definition->polar.mapRadius;
        state.moveStartTimeMs = t;    // Save initial time of move
        state.inMotion = true; // Start move on next frame
      }
    }
  }

  /// If autoPupils is enabled, periodically triggers a change in size of the pupils.
  void applyAutoPupils(const Eye<Disp> &eye) {
    if (!autoPupils) {
      return;
    }
    // Automated pupil resizing. Use autonomous iris w/fractal subdivision
    float n, sum = 0.5f;
    for (uint16_t i = 0; i < irisLevels; i++) {
      uint16_t iexp = 1 << (i + 1);         // 2,4,8,16,...
      uint16_t imask = (iexp - 1);          // 2^i-1 (1,3,7,15,...)
      uint16_t ibits = state.irisFrame & imask;  // 0 to mask
      if (ibits) {
        float weight = (float) ibits / (float) iexp; // 0.0 to <1.0
        n = irisPrev[i] * (1.0f - weight) + irisNext[i] * weight;
      } else {
        n = irisNext[i];
        irisPrev[i] = irisNext[i];
        irisNext[i] = -0.5f + ((float) random(1000) / 999.0f); // -0.5 to +0.5
      }
      iexp = 1 << (irisLevels - i); // ...8,4,2,1
      sum += n / (float) iexp;
    }

    const float irisMin = 1.0f - eye.definition->pupil.max;
    const float irisRange = eye.definition->pupil.max - eye.definition->pupil.min;
    state.irisValue = irisMin + sum * irisRange;

    if ((++state.irisFrame) >= (1 << irisLevels)) {
      state.irisFrame = 0;
    }
  }

  void wink(Eye<Disp> &eye, uint32_t duration) {
    if (eye.blink.state == BlinkState::NotBlinking) {
      eye.blink.state = BlinkState::BlinkClosing;
      eye.blink.startTimeMs = millis();
      eye.blink.durationMs = duration;
    }
  }

  uint32_t doBlink() {
    const uint32_t blinkDuration = random(50, 100); // Blink for a somewhat random number of milliseconds
    // Set up durations for all eyes (if not already winking)
    for (auto &e: eyes) {
      wink(e, blinkDuration);
    }
    return blinkDuration;
  }

  /// If autoBlink is enabled, periodically starts the eyes blinking.
  void applyAutoBlink() {
    if (!autoBlink) {
      return;
    }

    const uint32_t t = millis();
    if (t - state.timeOfLastBlinkMs >= state.timeToNextBlinkMs) {
      state.timeOfLastBlinkMs = t;
      const uint32_t blinkDuration = doBlink();
      state.timeToNextBlinkMs = blinkDuration * 3 + random(4000);
    }
  }

  /// Updates a blink state for the current point in time.
  /// \param blink the blink state to update.
  /// \return the amount of 'blink' that was calculated, where 0 means not blinking
  /// and 1 means the eye is fully closed.
  float updateBlinkState(Eye<Disp> &eye) {
    auto &blink = eye.blink;
    const uint32_t t = millis();
    float blinkFactor{};
    if (blink.state != BlinkState::NotBlinking) {
      // The eye is currently blinking. We scale the upper/lower thresholds relative
      // to the blink position, so that blinks work together with pupil tracking.
      if (t - blink.startTimeMs >= blink.durationMs) {
        // Advance to the next blink state
        switch (blink.state) {
          case BlinkState::BlinkClosing:
            blink.state = BlinkState::BlinkOpening;
            blink.durationMs *= 2; // Open the eyelids at half the speed they closed at
            blink.startTimeMs = t;
            blinkFactor = 1.0f;
            break;
          case BlinkState::BlinkOpening:
            blink.state = BlinkState::NotBlinking;
            blinkFactor = 0.0f;
            break;
          default:
            // This should never happen
            break;
        }
      } else {
        blinkFactor = (float) (t - blink.startTimeMs) / (float) blink.durationMs;
        if (blink.state == BlinkState::BlinkOpening) {
          blinkFactor = 1.0f - blinkFactor;
        }
      }
    }
    return blinkFactor;
  }

  float mapToScreen(int value, int mapRadius, int eyeRadius) const {
    return sinf((float) value / (float) mapRadius) * (float) M_PI_2 * (float) eyeRadius;
  }

  void constrainEyeCoord(float &x, float &y) {
    // If the coordinate doesn't fit in the unit circle, scale it down
    auto d2 = x * x + y * y;
    if (d2 > 1.0f) {
      auto d = sqrtf(d2);
      x /= d;
      y /= d;
    }
  }

  std::pair<float, float> computeEyelids(Eye<Disp> &eye) const {
    float upperQ, lowerQ;
    if (eye.definition->tracking) {
      // Eyelids are set to naturally "track" the pupils (move up or down automatically).
      // Find the pupil position on screen
      uint16_t mapRadius = eye.definition->polar.mapRadius;
      int ix = (int) mapToScreen(mapRadius - eye.x, mapRadius, eye.definition->radius) + screenWidth / 2;
      int iy = (int) mapToScreen(mapRadius - eye.y, mapRadius, eye.definition->radius) + screenWidth / 2;
      iy -= eye.definition->iris.radius * eye.definition->squint;
      if (eyeIndex & 1) {
        // Flip for right eye
        ix = screenWidth - 1 - ix;
      }
      uint8_t upperOpen = eye.definition->eyelids.upperOpen(ix);
      if (iy <= upperOpen) {
        upperQ = 1.0f;
      } else {
        uint8_t upperClosed = eye.definition->eyelids.upperClosed(ix);
        if (iy >= upperClosed) {
          upperQ = 0.0f;
        } else {
          upperQ = (float) (upperClosed - iy) / (float) (upperClosed - upperOpen);
        }
      }
      lowerQ = 1.0f - upperQ;
    } else {
      // No tracking - eyelids are fully open unless blink modifies them
      upperQ = 1.0f;
      lowerQ = 1.0f;
    }

    return std::pair<float, float>{upperQ, lowerQ};
  }

  /// Applies the current fixation amount to the given eye, so the eyes are slightly
  /// cross-eyed (giving the impression they're focusing in the near distance).
  /// Nothing is done if there is only one eye.
  void applyFixation(Eye<Disp> &eye) {
    if (eyes.size() == 1) {
      return;
    }
    state.fixate = ((state.fixate * 15) + nufix) / 16;
    eye.x = (eyeIndex & 1) ? eye.x - static_cast<float>(state.fixate) : eye.x + static_cast<float>(state.fixate);
  }

  /// Applies any spin (rotation) to the iris and sclera, based on the eye's definition
  /// and the current point in time.
  /// \param eye the eye to apply the spin to.
  void applySpin(Eye<Disp> &eye) {
    const float minutes = static_cast<float>(millis()) / 60000.0f;
    if (eye.definition->iris.iSpin) {
      // Spin works in fixed amount per frame (eyes may lose sync, but "wagon wheel" tricks work)
      eye.currentIrisAngle += eye.definition->iris.iSpin;
    } else {
      // Keep consistent timing in spin animation (eyes stay in sync, no "wagon wheel" effects)
      eye.currentIrisAngle = lroundf(
          static_cast<float>(eye.definition->iris.startAngle) + eye.definition->iris.spin * minutes * -1024.0f);
    }

    if (eye.definition->sclera.iSpin) {
      eye.currentScleraAngle += eye.definition->sclera.iSpin;
    } else {
      eye.currentScleraAngle = lroundf(
          static_cast<float>(eye.definition->sclera.startAngle) + eye.definition->sclera.spin * minutes * -1024.0f);
    }
  }

  /// Draws an entire eye to the screen.
  /// \param eye the eye to draw.
  /// \param upperFactor How open the upper eyelid is. 0 = fully closed, 1 = fully open.
  /// \param lowerFactor How open the lower eyelid is. 0 = fully closed, 1 = fully open.
  /// \param blinkFactor How much the eye is blinking. 0 = not blinking, 1 = fully blinking (closed).
  void drawEye(Eye<Disp> &eye, float upperFactor, float lowerFactor, float blinkFactor) {

    const uint8_t displacementMapSize = screenWidth / 2;
    const uint16_t mapRadius = eye.definition->polar.mapRadius;

    Disp &display = *eye.display;
    EyeBlink &blink = eye.blink;

    const int xPositionOverMap = eye.x - screenWidth / 2;
    const int yPositionOverMap = eye.y - screenHeight / 2;

    bool hasScleraTexture = eye.definition->sclera.hasTexture();
    bool hasIrisTexture = eye.definition->iris.hasTexture();

    const int pupilCheck = hasIrisTexture ? eye.definition->iris.texture.height : 1;
    const int iPupilFactor = (int) ((float) pupilCheck * 256 * (1.0 / state.irisValue));

    // Dampen the eyelid movement a bit
    upperFactor = eye.upperLidFactor * 0.7f + upperFactor * 0.3f;
    lowerFactor = eye.lowerLidFactor * 0.7f + lowerFactor * 0.3f;

    // Figure out how much the eyelids are open
    const float upperF = upperFactor * (1.0f - blinkFactor);
    const float lowerF = lowerFactor * (1.0f - blinkFactor);
    const float prevUpperF = eye.upperLidFactor * (1.0f - blink.blinkFactor);
    const float prevLowerF = eye.lowerLidFactor * (1.0f - blink.blinkFactor);

    // Store the state for the next iteration
    eye.upperLidFactor = upperFactor;
    eye.lowerLidFactor = lowerFactor;
    blink.blinkFactor = blinkFactor;

    for (uint16_t screenX = 0; screenX < screenWidth; screenX++) {
      // Determine the extents of the eye that need to be drawn, based on where the eyelids
      // are located in both this and the previous frame
      auto currentUpper = eye.definition->eyelids.upperLid(screenX, upperF);
      auto currentLower = eye.definition->eyelids.lowerLid(screenX, lowerF);

      uint8_t minY, maxY;
      if (eye.drawAll) {
        minY = 0;
        maxY = screenHeight;
      } else {
        auto previousUpper = eye.definition->eyelids.upperLid(screenX, prevUpperF);
        minY = std::min(currentUpper, previousUpper);
        auto previousLower = eye.definition->eyelids.lowerLid(screenX, prevLowerF);
        maxY = std::max(currentLower, previousLower);
      }

      // Figure out where we are in the displacement map. The eye (sphere) is symmetrical over
      // X and Y, so we can just swap axes to look up the Y displacement using the same table.
      const uint8_t *displaceX, *displaceY;
      int8_t xmul; // Sign of X displacement: +1 or -1
      int doff; // Offset into displacement arrays
      if (screenX < (screenWidth / 2)) {
        // Left half of screen, so we need to horizontally flip our displacement map lookup
        displaceX = &eye.definition->displacement[displacementMapSize - 1 - screenX];
        displaceY = &eye.definition->displacement[(displacementMapSize - 1 - screenX) * displacementMapSize];
        xmul = -1; // X displacement is always negative
      } else {
        // Right half of screen, so we can lookup horizontally as-is
        displaceX = &eye.definition->displacement[screenX - displacementMapSize];
        displaceY = &eye.definition->displacement[(screenX - displacementMapSize) * displacementMapSize];
        xmul = 1; // X displacement is always positive
      }

      const int xx = xPositionOverMap + screenX;
      for (uint16_t screenY = minY; screenY < maxY; screenY++) {
        uint16_t p;

        if (screenY < currentUpper || screenY >= currentLower) {
          // We're in the eyelid
          p = eye.definition->eyelids.color;
        } else {
          const int yy = yPositionOverMap + screenY;
          int dx, dy;
          if (screenY < displacementMapSize) {
            // We're in the top half of the screen, so we need to vertically flip the displacement map lookup
            doff = displacementMapSize - screenY - 1;
            dy = -displaceY[doff];
          } else {
            // We're in the bottom half of the screen
            doff = screenY - displacementMapSize;
            dy = displaceY[doff];
          }
          dx = displaceX[doff * displacementMapSize];
          if (dx < 255) {
            // We're inside the eyeball (sclera/iris/pupil) area
            dx *= xmul;  // Flip x offset sign if in left half of screen
            int mx = xx + dx;
            int my = yy + dy;

            if (mx >= 0 && mx < mapRadius * 2 && my >= 0 && my < mapRadius * 2) {
              // We're inside the polar angle/distance maps
              uint16_t angle;
              int distance, moff;
              if (my >= mapRadius) {
                my -= mapRadius;
                if (mx >= mapRadius) {
                  // Quadrant 1 (bottom right), so we can use the angle/dist lookups directly
                  mx -= mapRadius;
                  moff = my * mapRadius + mx;
                  angle = eye.definition->polar.angle[moff];
                  distance = eye.definition->polar.distance[moff];
                } else {
                  // Quadrant 2 (bottom left), so rotate angle by 270 degrees clockwise (768) and mirror distance on X axis
                  mx = mapRadius - mx - 1;
                  angle = eye.definition->polar.angle[mx * mapRadius + my] + 768;
                  distance = eye.definition->polar.distance[my * mapRadius + mx];
                }
              } else {
                if (mx < mapRadius) {
                  // Quadrant 3 (top left), so rotate angle by 180 degrees and mirror distance on the X and Y axes
                  mx = mapRadius - mx - 1;
                  my = mapRadius - my - 1;
                  moff = my * mapRadius + mx;
                  angle = eye.definition->polar.angle[moff] + 512;
                  distance = eye.definition->polar.distance[moff];
                } else {
                  // Quadrant 4 (top right), so rotate angle by 90 degrees clockwise (256) and mirror distance on Y axis
                  mx -= mapRadius;
                  my = mapRadius - my - 1;
                  angle = eye.definition->polar.angle[mx * mapRadius + my] + 256;
                  distance = eye.definition->polar.distance[my * mapRadius + mx];
                }
              }

              // Convert the polar angle/distance to texture map coordinates
              if (distance < 128) {
                // We're in the sclera
                if (hasScleraTexture) {
                  angle = ((angle + eye.currentScleraAngle) & 1023) ^ eye.definition->sclera.mirror;
                  const int tx = (angle & 1023) * eye.definition->sclera.texture.width / 1024; // Texture map x/y
                  const int ty = distance * eye.definition->sclera.texture.height / 128;
                  p = eye.definition->sclera.texture.get(tx, ty);
                } else {
                  p = eye.definition->sclera.color;
                }
              } else if (distance < 255) {
                // Either the iris or pupil
                const int ty = (distance - 128) * iPupilFactor / 32768;
                if (ty >= pupilCheck) {
                  // Pupil
                  p = eye.definition->pupil.color;
                } else {
                  // Iris
                  if (hasIrisTexture) {
                    angle = ((angle + eye.currentIrisAngle) & 1023) ^ eye.definition->iris.mirror;
                    const int tx = (angle & 1023) * eye.definition->iris.texture.width / 1024;
                    p = eye.definition->iris.texture.get(tx, ty);
                  } else {
                    p = eye.definition->iris.color;
                  }
                }
              } else {
                // Back of eye
                p = eye.definition->backColor;
              }
            } else {
              // We're outside the polar map so just use the back-of-eye color
              p = eye.definition->backColor;
            }
          } else {
            // We're outside the eye area, i.e. this must be on an eyelid
            p = eye.definition->eyelids.color;
          }
        }
        display.drawPixel(screenX, screenY, p);
      } // end column
    } // end scanline

    eye.drawAll = false;

    display.update();
  }

  Eye<Disp> &currentEye() {
    return eyes[eyeIndex];
  }

public:
  EyeController(std::array<DisplayDefinition<Disp>, numEyes> displayDefs, bool autoMove, bool autoBlink,
                bool autoPupils) :
      autoMove(autoMove), autoBlink(autoBlink), autoPupils(autoPupils) {
    size_t i{};
    for (const auto &dispDef: displayDefs) {
      Eye<Disp> &eye = eyes[i++];
      eye.display = dispDef.display;
      updateDefinition(eye, dispDef.definition);
      eye.blink.state = BlinkState::NotBlinking;

      // Start with the eyes looking straight ahead
      eye.x = eye.y = state.eyeOldX = state.eyeNewX = state.eyeOldY = state.eyeNewY = eye.definition->polar.mapRadius;

      eye.drawAll = true;
    }
  }

  /// Enables or disables the automatic random eye movement.
  /// \param enabled true to enable automatic movement, false to disable it.
  void setAutoMove(bool enabled) {
    autoMove = enabled;
  }

  /// \return true if automatic eye movement is enabled, false otherwise.
  bool autoMoveEnabled() {
    return autoMove;
  }

  /// Enables or disables automatic random eye blinking.
  /// \param enabled true to enable automatic blinking, false to disable it.
  void setAutoBlink(bool enabled) {
    autoBlink = enabled;
  }

  /// \return true if automatic blinking is enabled, false otherwise.
  bool autoBlinkEnabled() {
    return autoBlink;
  }

  /// Enables or disables automatic random pupil resizing.
  /// \param enabled true to enable automatic random pupil resizing, false to disable it.
  void setAutoPupils(bool enabled) {
    autoPupils = enabled;
  };

  /// \return true if automatic pupil resizing is enabled, false otherwise.
  bool autoPupilsEnabled() {
    return autoPupils;
  }

  /// Sets the maximum amount of time the eyes will remain still when autoMove is enabled.
  /// \param maxGazeMillis the maximum time to look in a single direction, in milliseconds.
  void setMaxGazeMs(uint32_t maxGazeMillis) {
    maxGazeMs = maxGazeMillis;
  }

  void updateDefinition(Eye<Disp> &eye, const EyeDefinition &def) {
    eye.definition = &def;
    eye.currentIrisAngle = def.iris.startAngle;
    eye.currentScleraAngle = def.sclera.startAngle;
    // Draw the entire eye (including eyelids) on the first frame
    eye.drawAll = true;
  }

  /// Starts the blink process, if the eye(s) are not already blinking.
  void blink() {
    doBlink();
    state.timeToNextBlinkMs = 0;
  }

  /// Starts winking the eye at the given index. If the index is out of range or the eye
  /// is already winking or blinking, nothing will happen.
  void wink(size_t index) {
    if (index < eyes.size()) {
      wink(eyes[index], random(50, 100));
    }
  }

  /// Sets the target pupil location. The eye(s) will smoothly move to this position over time. The coordinate
  /// should fall inside the unit circle. If it falls outside, the closest point on the unit circle's perimeter
  /// will be used instead.
  /// \param xTarget the target x location for the eye(s), in the range -1.0 (hard left) to 1.0 (hard right)
  /// \param yTarget the target y location for the eye(s), in the range -1.0 (fully up) to 1.0 (fully down)
  void setTargetPosition(float xTarget, float yTarget, int moveDurationMs = 120) {
    constrainEyeCoord(xTarget, yTarget);
    Eye<Disp> &eye = currentEye();
    auto middle = static_cast<float>(eye.definition->polar.mapRadius);
    auto r = (middle * 2.0f - static_cast<float>(screenWidth) * static_cast<float>(M_PI_2)) * 0.75f;
    state.eyeNewX = middle - xTarget * r;
    state.eyeNewY = middle - yTarget * r;
    state.inMotion = true;
    state.moveDurationMs = moveDurationMs;
    state.moveStartTimeMs = millis();
  }

  /// Instantly moves the eye(s) to a given position. The coordinate should fall inside the unit circle.
  /// If it falls outside, the closest point on the unit circle's perimeter will be used instead.
  /// \param x the new x location for the eye(s), in the range -1.0 (hard left) to 1.0 (hard right)
  /// \param y the new y location for the eye(s), in the range -1.0 (fully up) to 1.0 (fully down)
  void setPosition(float x, float y) {
    constrainEyeCoord(x, y);
    Eye<Disp> &eye = currentEye();
    auto middle = static_cast<float>(eye.definition->polar.mapRadius);
    auto r = (middle * 2.0f - static_cast<float>(screenWidth) * static_cast<float>(M_PI_2)) * 0.75f;
    state.eyeOldX = middle - x * r;
    state.eyeOldY = middle - y * r;
  }

  /// Sets the target pupil size. The pupil will smoothly expand or contract to this size over time.
  /// \param ratio a value between 0 and 1, where 0 is the smallest permissible pupil size,
  /// 1 is the largest.
  void setPupil(float ratio) {
    // TODO - implement me
//  const float irisMin = 1.0f - eye.definition->pupil.max;
//  const float irisRange = eye.definition->pupil.max - eye.definition->pupil.min;
//  state.irisValue = irisMin + sum * irisRange;
    state.irisValue = ratio;
  }

  /// Instantly resizes the pupil to the specified size.
  /// \param ratio a value between 0 and 1, where 0 is the smallest permissible pupil size,
  /// 1 is the largest.
  void setPupilInstant(float ratio) {
    // TODO - implement me
  }

  /// Updates the definitions of the eyes.
  /// \param definitions this must contain exactly one EyeDefinition for each eye that was
  /// defined in the constructor.
  void updateDefinitions(const std::array<EyeDefinition, numEyes> &definitions) {
    size_t i = 0;
    for (const EyeDefinition &def: definitions) {
      updateDefinition(eyes[i++], def);
    }
  }

  /// Renders a single frame of animation. If there is more than one eye defined,
  /// only a single eye/display will be updated.
  /// \return true if the rendering took place, false if it didn't (for example, because
  /// the display was not yet ready for drawing on because it is still asynchronously
  /// drawing the previous frame).
  bool renderFrame() {
    auto &eye = currentEye();

    if (!eye.display->isAvailable()) {
      return false;
    }

    // Apply any automated eye/eyelid/pupil movements
    applyAutoMove(eye);
    applyAutoBlink();
    applyAutoPupils(eye);

    // Apply any time-based changes
    auto blinkFactor = updateBlinkState(eye);
    applyFixation(eye);
    applySpin(eye);

    const std::pair<float, float> &pair = computeEyelids(eye);
    //auto [upperQ, lowerQ] = computeEyelids(eye);
    auto upperQ = pair.first;
    auto lowerQ = pair.second;

    // Draw the eye. We temporarily flip the X value if this is the right eye, since it is mirrored
    if (eyeIndex == 0) eye.x = eye.definition->polar.mapRadius * 2 - eye.x;
    drawEye(eye, upperQ, lowerQ, blinkFactor);
    // Flip it back
    if (eyeIndex == 0) eye.x = eye.definition->polar.mapRadius * 2 - eye.x;

    // Render the updated eye to its screen
    eye.display->update();

    // Advance to the next eye for the next frame
    eyeIndex = (eyeIndex + 1) % eyes.size();

    return true;
  }
};
