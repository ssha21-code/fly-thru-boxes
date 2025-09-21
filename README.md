# fly-thru-boxes
This is a program made in Raylib C++. It is a small, fun game made in C++.
The Rocket is the player.
It is a game where a Rocket flies through Crates and tries to dodge them.
The Rocket can have a maximum of 3 lives.
The Rocket can shoot infinite number of regular yellow Turrets and can shoot limited green Special Turrets based on the ammo of the Rocket.
The Special Turrets are to be used sparingly and only in emergencies, like when you don't have enough time to reach the opening of the crates.
The types of PowerUps are: 
* Yellow PowerUp: This PowerUp adds an extra life to the Rocket at the maximum of three lives.
* Green PowerUp: This PowerUp adds two points to the score.
* Red PowerUp: This PowerUp adds one ammo to the Rocket. This PowerUp can only be obtained by a UFO dying or a Golden Crate being shot.
Regular Crates are only vulnerable to green Turrets.
A PowerUp can spawn every three rounds of Crates.
When a Rocket collects those PowerUps, they get benefits.
Every five rounds of crates, a Golden Crate spawns.
When the Golden Crate is shot with either type of Turret, 2 green PowerUps, one red PowerUp, and enough yellow PowerUps to bring the Rocket to full health.
Every eleven rounds of Crates, a UFO spawns along side the Crates.
This UFO can shoot at the player every two seconds, and each Turret of the UFO reduces the player's life count.
UFO's have only 10HP, and take 1HP of damage from regular Rocket Turrets, and 2HP from Special Rocket Turrets. The UFO cannot shoot Special Turrets.
Once the UFO gets defeated, it drops a hefty amount of PowerUps: four Green PowerUps, one Yellow PowerUp, and two Red PowerUps. Then, it disappears and reappears in the next eleventh round.
If by any chance, the Rocket loses all of its lives, the game will reset all stats and go to the starting screen.
The controls of the game are:
* Shoot Regular Turret: Spacebar
* Move Left: A
* Move Right: D
* Shoot Special Turret: W
This game can be played infinitely, meaning there is no end, as long as the Rocket does not die.
The goal is to dodge all Crates, collect PowerUps, carefully maneuver around the Crates and shoot Golden Crates and collect its drops, maintain all three lives, use Special Turrets wherever necessary, shoot the UFO's, and kill them and collect their drops, and to get a score as high as possible.
The game contains a lot of images, so I have made a release that has all images built into the executable through xxd-ing all the images into includes and importing the image from there.
I hope all players have as much fun playing my game, just as how much fun I had designing it. 😉
