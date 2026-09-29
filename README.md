# Carrion Watch 0.1.0

DayZ Standalone mod. Birds do not care about your loot. They care that you stopped moving.

After a player dies, a thin smoke column and a carrion call gather over the body. Anyone on a ridge can see or hear that something died. Shooting near the flock scatters it for two minutes and tells everyone *you* are still there.

No traders. No new guns. No types.xml.

Repo: https://github.com/Duke19941/CarrionWatch

## Loop

1. Player dies.
2. Server waits **90 seconds** (body can still be looted quietly).
3. A `CW_CarrionFlock` marker is spawned on the corpse.
4. Clients play camp smoke + swarming flies + a looping call (range ~420 m).
5. Flock follows the corpse if it is dragged.
6. After **12 minutes**, or when the body is deleted, the flock leaves.
7. A gunshot within **35 m**, or a bullet hitting the marker, scatters the flock for **120 seconds**.

## Tune

First server boot writes `$profile:CarrionWatch/settings.json`.

| Key | Default | Meaning |
| --- | --- | --- |
| `delaySeconds` | 90 | Quiet window after death |
| `lifetimeSeconds` | 720 | How long the tell lasts |
| `scatterSeconds` | 120 | How long gunfire empties the sky |
| `gunshotRadius` | 35 | Metres around the body |
| `enableSmoke` / `enableFlies` / `enableCall` | true | Client FX switches |
| `playersOnly` | true | Ignore infected / animals |

## Pack

See `Tools/pack_notes.txt`. Prefixes must be `CarrionWatch\Scripts` and `CarrionWatch\Data`.

Confirm in `%LOCALAPPDATA%\DayZ\script_*.log`:

```
[CarrionWatch] Server ready.
[CarrionWatch] Death queued.
[CarrionWatch] Flock spawned
```

## Audio note

`Data/config.cpp` points the call at vanilla crow sample paths. If those files are missing in your build, the engine skips the shader and you still get smoke + flies.

## What this is not

- Not a loot beacon you can pick up. The marker cannot go into hands or cargo.
- Not a zombie feature. v0 is player corpses only.
- Not CF / Expansion. One script PBO, one data PBO.
