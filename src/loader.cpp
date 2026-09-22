/*
 * mod-baseline-buffs loader.
 *
 * The playerbots fork auto-globs every module's sources into one lib and looks
 * up a loader symbol derived from the folder name: for folder "mod-baseline-buffs"
 * that symbol is exactly "Addmod_baseline_buffsScripts". It must exist and call our
 * real registration function.
 *
 * Released under GNU GPL v2 or (at your option) any later version.
 */

void AddBaselineBuffsScripts();

void Addmod_baseline_buffsScripts()
{
    AddBaselineBuffsScripts();
}
