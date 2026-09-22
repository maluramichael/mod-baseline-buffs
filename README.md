# mod-baseline-buffs

An [AzerothCore](https://www.azerothcore.org/) module (WotLK 3.3.5a) that turns three
class utility buffs into **baseline abilities** — available without spending a talent point.

## What it does

On login and on level-up, a character automatically learns the appropriate rank of its
utility buff:

| Class   | Buff              |
|---------|-------------------|
| Priest  | Divine Spirit     |
| Paladin | Blessing of Kings |
| Druid   | Mark of the Wild  |

Spells are granted only when not already known, always at the highest rank the character's
level allows. Works for real players and bots alike. No SQL and no client patch required.

## Configuration

`conf/mod_baseline_buffs.conf.dist`:

| Key                       | Default | Description                  |
|---------------------------|---------|------------------------------|
| `BaselineBuffs.Enable`    | `1`     | Master on/off switch         |
| `BaselineBuffs.Priest`    | `1`     | Grant Divine Spirit          |
| `BaselineBuffs.Paladin`   | `1`     | Grant Blessing of Kings      |
| `BaselineBuffs.Druid`     | `1`     | Grant Mark of the Wild       |

## Installation

Clone into your AzerothCore `modules/` directory and rebuild the worldserver.

## License

Released under the GNU GPL v2 (or later).
