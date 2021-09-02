#ifndef __PLAYER_H
#define __PLAYER_H

VOID PlayerInit(VOID);

VOID PlayerOnKeyUp(
  IN ULONG dwKey
  );

VOID PlayerOnKeyDown(
  IN FLOAT fDeltaTime,
  IN ULONG dwKey
  );

VOID PlayerUpdate(
  IN FLOAT fDeltaTime
  );

VOID PlayerDraw(VOID);

#endif
