# VIRTUAL GUY

![wallpaper](https://github.com/OCA99/PlatformerGame/blob/master/Images/wallpaper.PNG?raw=true)

## Description

A platformer game prototype made by students from the CITM Barcelona using SDL. This prototype is part of the Game Development course from the CITM UPC Barcelona, taught by Ramon Santamaria in 2020-2021.

## Key Features

 - Game config loading from XML.
 - Game state saving and loading with XML.
 - Map loading using TMX files.
 - Full dynamic collision detection.
 - Player movement, jumping and dashing.
 - Menu screens including logo, title and ending.
 - Player animations.
 - Player double jump.
 - Player dash skill with cooldown.
 - Two full levels.
 - Parallax background.
 - Flying enemies that can pathfind to the player.
 - Walking enemies that can pathfind, jump and drop to the player.
 - All enemies have a detection range.
 - FPS capped at 60.
 - All movement AND animations use delta time.
 - The player can destroy enemies by jumping onto them.
 - Collectible hearts to recover lives.
 - Collectible fruits to increase score.
 - Simple UI to display lives, score and debug status.
 - Checkpoints with autosave when passing them.
 - GUI menu for easy teleport between checkpoints.
 - Ability to throw knives at enemies.
 - Audio feedback for nearly all interactions.
 - Debug keys to visualize collisions, paths, change levels, and set god-mode for player.
 - Profiling funcionality using [Optick](https://github.com/bombomby/optick).
 
## Controls

 - Walk left: A.
 - Walk right: D.
 - Jump & Double jump: Space.
 - Use dash skill: F.
 - Throw knife: Left arrow / Right arrow.
 - Open / close checkpoint teleport menu: T
  - Select checkpoint to teleport to: Left arrow / Right arrow.
  - Teleport to selected checkpoint: Return key.
 - F1/F2: Start from first / second level.
 - F3: Restart current level.
 - F5: Save current game state.
 - F6: Load previous game state.
 - F9: View colliders, paths and logic.
 - F10: God Mode.
  - God Mode movement: WASD.
 - F11: Enable / Disable FPS cap to 30.

## Developers

 - Oscar Cuatrecases - developer.
 - Martí Davicino - developer, art, testing.
 - Eduard Minguell - developer, art.


## License

This project is licensed under an unmodified MIT license, which is an OSI-certified license that allows static linking with closed source software. Check [LICENSE](LICENSE.md) for further details.

[SDL License](https://www.libsdl.org/license.php)

[PUGI License](https://pugixml.org/license.html)

