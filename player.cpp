#define _USE_MATH_DEFINES

#include <math.h>
#include "engine.h"
#include "player.h"
#include "asteroid.h"

#define PLAYER_HALF_WIDTH  10
#define PLAYER_HALF_HEIGHT 10

static FLOAT fPlayerX;
static FLOAT fPlayerY;
static FLOAT fPlayerXVelocity = 0.0f;
static FLOAT fPlayerYVelocity = 0.0f;
static FLOAT fScreenWidth;
static FLOAT fScreenHeight;
static BOOL  bPlasmaBeam = FALSE;
static FLOAT fPlasmaBeamRecharge = 0.0f;
static FLOAT fPlasmaBeamTime = 0.0f;

static D2D1_POINT_2F PlayerPoints[] = {
  {  0, PLAYER_HALF_HEIGHT * 0.5 },
  { -PLAYER_HALF_WIDTH, PLAYER_HALF_HEIGHT },
  {  0, -PLAYER_HALF_HEIGHT },
  {  PLAYER_HALF_WIDTH, PLAYER_HALF_HEIGHT },
};

VOID PlayerInit(VOID) {
  D2D_SIZE_F WindowSize = Engine::RenderTarget->GetSize();

  fPlayerX = (FLOAT)WindowSize.width / 2;
  fPlayerY = (FLOAT)WindowSize.height * 0.95;
  fScreenWidth = WindowSize.width;
  fScreenHeight = WindowSize.height;
}

VOID PlayerOnKeyUp(
  IN ULONG dwKey
  )
{
  if (dwKey == VK_SPACE) {
    bPlasmaBeam = FALSE;
  }
}

VOID PlayerOnKeyDown(
  IN FLOAT fDeltaTime,
  IN ULONG dwKey
  )
{
  #define CAP 3

  if (dwKey == VK_LEFT || dwKey == 'A') {
    fPlayerXVelocity -= fDeltaTime * 50;
  } else if (dwKey == VK_RIGHT || dwKey == 'D') {
    fPlayerXVelocity += fDeltaTime * 50;
  } else if (dwKey == VK_SPACE) {
    bPlasmaBeam = TRUE;
  }

  if (fPlayerXVelocity >= CAP) {
    fPlayerXVelocity = CAP;
  } else if (fPlayerXVelocity <= -CAP) {
    fPlayerXVelocity = -CAP;
  }

  if (fPlayerYVelocity >= CAP) {
    fPlayerYVelocity = CAP;
  } else if (fPlayerYVelocity <= -CAP) {
    fPlayerYVelocity = -CAP;
  }

  #undef CAP
}

VOID PlayerUpdate(
  IN FLOAT fDeltaTime
  )
{
  fPlayerX += fPlayerXVelocity;
  fPlayerY += fPlayerYVelocity;

  if (bPlasmaBeam) {
    if (fPlasmaBeamTime <= 1.0f) {
      fPlasmaBeamTime += fDeltaTime * 5;
    } else {
      bPlasmaBeam = FALSE;
    }
  }

  if (fPlasmaBeamTime > 1.0f) {
    fPlasmaBeamRecharge += fDeltaTime * 1;

    if (fPlasmaBeamRecharge >= 1.0f) {
      fPlasmaBeamTime = 0.0f;
      fPlasmaBeamRecharge = 0.0f;
    }
  }

  if (fPlayerX <= PLAYER_HALF_WIDTH) {
    fPlayerX = PLAYER_HALF_WIDTH;
    fPlayerXVelocity = 0;
  } else if (fPlayerX >= fScreenWidth - PLAYER_HALF_WIDTH) {
    fPlayerX = fScreenWidth - PLAYER_HALF_WIDTH;
    fPlayerXVelocity = 0;
  }

  if (bPlasmaBeam) {
    for (ULONG_PTR i = 0; i != ARRAYSIZE(AsteroidObjects); i++) {
      if (!AsteroidObjects[i]) {
        continue;
      }

      if ((AsteroidObjects[i]->X - AsteroidObjects[i]->dwRadius) <= fPlayerX && (AsteroidObjects[i]->X + AsteroidObjects[i]->dwRadius) >= fPlayerX) {
        free(AsteroidObjects[i]);
        AsteroidObjects[i] = NULL;
      }
    }
  }
}

VOID PlayerDraw(VOID) {
  Engine::RenderTarget->SetTransform(D2D1::Matrix3x2F::Identity() * D2D1::Matrix3x2F::Translation(fPlayerX, fPlayerY));

  for (ULONG_PTR i = 0; i != ARRAYSIZE(PlayerPoints); i++) {
    Engine::RenderTarget->DrawLine(PlayerPoints[i], PlayerPoints[i == ARRAYSIZE(PlayerPoints) - 1 ? 0 : i + 1], Engine::WhiteBrush, 1.0f, NULL);
  }

  if (bPlasmaBeam) {
    D2D1_POINT_2F BeamStart = { 0, -PLAYER_HALF_HEIGHT, };
    D2D1_POINT_2F BeamEnd = { 0, -fScreenHeight };

    Engine::RenderTarget->DrawLine(BeamStart, BeamEnd, Engine::WhiteBrush);
  }
}
