#ifndef __ENEMY_H
#define __ENEMY_H

#define ASTEROIDS_MAX 16

typedef struct _ASTEROID_OBJECT {
  FLOAT X;
  FLOAT Y;
  FLOAT fAngle;
  FLOAT fRotateSpeed;
  FLOAT fYVelocity;
  ULONG dwVerticesCount;
  LONG  dwRadius;
  D2D1_POINT_2F Vertices[ANYSIZE_ARRAY];
} ASTEROID_OBJECT, * PASTEROID_OBJECT;

extern PASTEROID_OBJECT AsteroidObjects[ASTEROIDS_MAX];

VOID AsteroidInit(VOID);

VOID AsteroidUpdate(
  IN FLOAT fDeltaTime
  );

VOID AsteroidDraw(VOID);

#endif
