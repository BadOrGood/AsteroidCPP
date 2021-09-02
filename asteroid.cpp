#define _USE_MATH_DEFINES

#include <math.h>
#include "asteroid.h"
#include "engine.h"

#define ASTEROID_VERTICES_MAX   11
#define ASTEROID_VERTICES_MIN   5
#define ASTEROID_RADIUS_MIN     5
#define ASTEROID_RADIUS_MAX     50
#define ASTEROID_ROTATE_MIN     10
#define ASTEROID_ROTATE_MAX     200
#define ASTEROID_Y_VELOCITY_MIN 10
#define ASTEROID_Y_VELOCITY_MAX 200
#define ASTEROID_SPAWN_TICK     60

static FLOAT fScreenWidth;
static FLOAT fScreenHeight;

PASTEROID_OBJECT AsteroidObjects[ASTEROIDS_MAX];

static VOID AsteroidGenerate(VOID) {
  BOOL  bFoundCave = FALSE;
  ULONG dwVerticesCount = ASTEROID_VERTICES_MIN + (rand() % (ASTEROID_VERTICES_MAX - ASTEROID_VERTICES_MIN));
  FLOAT fTotalDegrees = 360;
  PASTEROID_OBJECT Asteroid = (PASTEROID_OBJECT)malloc(sizeof(ASTEROID_OBJECT) + (sizeof(D2D1_POINT_2F) * dwVerticesCount));
  D2D1_POINT_2F * VerticesIndex = Asteroid->Vertices;
  ULONG dwAnglePart = 360 / dwVerticesCount;
  ULONG dwAngle = 0;
  
  Asteroid->X = rand() % (ULONG)fScreenWidth;
  Asteroid->Y = -ASTEROID_RADIUS_MAX;
  Asteroid->fAngle = 0.0f;
  Asteroid->dwVerticesCount = dwVerticesCount;
  Asteroid->fYVelocity = ASTEROID_Y_VELOCITY_MIN + (rand() % (ASTEROID_Y_VELOCITY_MAX - ASTEROID_Y_VELOCITY_MIN));

  if (rand() & 1) { 
    Asteroid->fRotateSpeed = ASTEROID_ROTATE_MIN + (rand() % (ASTEROID_ROTATE_MAX - ASTEROID_ROTATE_MIN));
  } else {
    Asteroid->fRotateSpeed = -(ASTEROID_ROTATE_MIN + (rand() % (ASTEROID_ROTATE_MAX - ASTEROID_ROTATE_MIN)));
  }

  for (ULONG_PTR i = 0; i != ARRAYSIZE(AsteroidObjects); i++) {
    if (!AsteroidObjects[i]) {
      AsteroidObjects[i] = Asteroid;
      bFoundCave = TRUE;

      break;
    }
  }

  if (!bFoundCave) {
    free(Asteroid);

    return;
  }

  for (ULONG_PTR i = 0; i != (ULONG_PTR)dwVerticesCount; i++) {
    FLOAT fAngle = dwAngle;
    FLOAT fXRadius = 0;

    if ((fXRadius = (ASTEROID_RADIUS_MIN + (rand() % (ASTEROID_RADIUS_MAX - ASTEROID_RADIUS_MIN)))) > Asteroid->dwRadius) {
      Asteroid->dwRadius = fXRadius;
    }

    dwAngle += dwAnglePart;

    VerticesIndex->x = cosf((M_PI / 180) * fAngle) * fXRadius;
    VerticesIndex->y = sinf((M_PI / 180) * fAngle) * (ASTEROID_RADIUS_MIN + (rand() % (ASTEROID_RADIUS_MAX - ASTEROID_RADIUS_MIN)));
    VerticesIndex++;
  }
}

VOID AsteroidInit(VOID) {
  D2D1_SIZE_F ScreenSize = Engine::RenderTarget->GetSize();

  srand((ULONG)GetTickCount64());

  fScreenWidth = ScreenSize.width;
  fScreenHeight = ScreenSize.height;

  AsteroidGenerate();
}

VOID AsteroidUpdate(
  IN FLOAT fDeltaTime
  )
{
  static ULONG dwSpawnTick = 0;

  if (++dwSpawnTick > ASTEROID_SPAWN_TICK) {
    AsteroidGenerate();

    dwSpawnTick = 0;
  }

  for (ULONG_PTR i = 0; i != ARRAYSIZE(AsteroidObjects); i++) {
    if (!AsteroidObjects[i]) {
      continue;
    }

    PASTEROID_OBJECT Asteroid = AsteroidObjects[i];

    Asteroid->fAngle += Asteroid->fRotateSpeed * fDeltaTime;
    Asteroid->Y += Asteroid->fYVelocity * fDeltaTime;

    if (Asteroid->Y > fScreenHeight + ASTEROID_RADIUS_MAX) {
      AsteroidObjects[i] = NULL;

      free(Asteroid);
    }
  }
}

VOID AsteroidDraw(VOID) {
  for (ULONG_PTR i = 0; i != ARRAYSIZE(AsteroidObjects); i++) {
    if (!AsteroidObjects[i]) {
      continue;
    }

    PASTEROID_OBJECT Asteroid = AsteroidObjects[i];

    Engine::RenderTarget->SetTransform(D2D1::Matrix3x2F::Identity() * D2D1::Matrix3x2F::Translation(Asteroid->X, Asteroid->Y) * D2D1::Matrix3x2F::Rotation(Asteroid->fAngle, D2D1_POINT_2F { Asteroid->X, Asteroid->Y }));

    for (ULONG_PTR y = 0; y != Asteroid->dwVerticesCount; y++) {
      Engine::RenderTarget->DrawLine(Asteroid->Vertices[y], Asteroid->Vertices[y == Asteroid->dwVerticesCount - 1 ? 0 : y + 1], Engine::WhiteBrush);
    }
  }
}
