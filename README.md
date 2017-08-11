# CarND-Controls-PID
Self-Driving Car Engineer Nanodegree Program

---
# Effects of PID components
![alt text](pid.png "PID formula")

The formula above represents the generic representation of a PID controller for our project. CTE, is the 'Cross Track Error', the difference between the desired trajectory of the vehicle and our current position. The tau parameters are the hyperparameters to tune, in our project they are represented as Kp, Ki and Kd. 

## P - Proportional
Adjusting Kp (main.cpp:39, PID.cpp:16) controls the 'proportional' part of the controller. As the error gets larger this part will grow proportionally making the correctiong bigger too. 


## I - Integral 
Adjusting Ki we control the integral part of the PID controller. If there is any kind of constant bias in the system this part of the controller will compensate for it over time.

## D - Differential
In order to dampen the quick oscillations introduced by the P controller, the differential controller and its factor Kd, adjust the rate according to the derivate of the CTE or the 'speed' of change of CTE. Bigger quick drifts from CTE will yield a quicker correction.

Here's a good graphic from [wikipedia](https://en.wikipedia.org/wiki/PID_controller) that shows the effects of modifying the parameters individually.

![pidpars](PID_Compensation_Animated.gif)

### PID in our project
The behavior of modifying the parameters individually can be seen like in the discussion above with our vehicle. For example [Here's a video](p_only.mp4) of my project code running only with a 'proportional controller' in this case of value 0.1. It can be seen how the vehicle oscilates before settling into the final value. This is the expected behavior when using a P controller only 

# Hyperpars optimization
The following is a discussion about how the final parameters were chosing for this project.

## Deciding against Twiddle/SGD
While the twiddle algorithm seems well suited to find good matches for the project there are significant challenges for running it though. Namely:
* The error to adjust every twiddle iteration requires a full lap of the track, and twiddle requires a significant number of iterations to converge losing practicality
* The simulator process is **inherently unstable** and requires manual supervision all along. Given that any iteration of twiddle yield parameters that take the vehicle off track, process can't be easily automated for many iterations and has to be manually supervised.
* A **real world** case for PID hyperpars tuning would present the same challenges making the case for scripting out hyperpars optimization for the sake of this project weaker.

Because of this I chose to do manual tuning.

## Manual tuning
Given that the system has to be online when tuning parameters it does make sense to use manual tuning. For that end we followed the following algorithm for adjusting manually:

1. Set Ki and Kd to 0
2. Increase Kp until vehicle oscilates, then halve Kp.
3. Increase Ki until any bias is removed
4. Increase Kd until vehicle is reasonable quick to react to quick oscilations but without overshooting

Also, here's compiled the effect of modifying every parameter independently:


Parameter | Rise time | Overshoot | Settling time | Steady-state error | Stability
---|---|---|---|---|---
Kp | Decrease | Increase | Small change | Decrease | Degrade
Ki | Decrease | Increase | Increase | Eliminate | Degrade
Kd | Minor change | Decrease|Decrease| No effect | Improve if Kd small

After following the above heuristic the following parameters were chosen for this project. Throttle setting = 0.6, this results in speed of approximately 60mph.

* **Kp = 0.1**
* **Ki = 0.00001**
* **Kd = 6**

[Here's a video](pid_video.mp4) of the track running with these parameters

## Final Considerations
* Once manual pars were chosen there seems to be opportunity for a modified Twiddle algorithm that can work online with minimal supervision. If we assume that the manually found parameters are close to a local minima in the parameter space, we could slowly modify Kp,Ki and Kd in the direction of the gradient. Because we start with a stable vehicle/track configuration we would likely not veer off track















# Project info
## Dependencies

* cmake >= 3.5
 * All OSes: [click here for installation instructions](https://cmake.org/install/)
* make >= 4.1
  * Linux: make is installed by default on most Linux distros
  * Mac: [install Xcode command line tools to get make](https://developer.apple.com/xcode/features/)
  * Windows: [Click here for installation instructions](http://gnuwin32.sourceforge.net/packages/make.htm)
* gcc/g++ >= 5.4
  * Linux: gcc / g++ is installed by default on most Linux distros
  * Mac: same deal as make - [install Xcode command line tools]((https://developer.apple.com/xcode/features/)
  * Windows: recommend using [MinGW](http://www.mingw.org/)
* [uWebSockets](https://github.com/uWebSockets/uWebSockets)
  * Run either `./install-mac.sh` or `./install-ubuntu.sh`.
  * If you install from source, checkout to commit `e94b6e1`, i.e.
    ```
    git clone https://github.com/uWebSockets/uWebSockets 
    cd uWebSockets
    git checkout e94b6e1
    ```
    Some function signatures have changed in v0.14.x. See [this PR](https://github.com/udacity/CarND-MPC-Project/pull/3) for more details.
* Simulator. You can download these from the [project intro page](https://github.com/udacity/self-driving-car-sim/releases) in the classroom.

There's an experimental patch for windows in this [PR](https://github.com/udacity/CarND-PID-Control-Project/pull/3)

## Basic Build Instructions

1. Clone this repo.
2. Make a build directory: `mkdir build && cd build`
3. Compile: `cmake .. && make`
4. Run it: `./pid`. 

## Editor Settings

We've purposefully kept editor configuration files out of this repo in order to
keep it as simple and environment agnostic as possible. However, we recommend
using the following settings:

* indent using spaces
* set tab width to 2 spaces (keeps the matrices in source code aligned)

## Code Style

Please (do your best to) stick to [Google's C++ style guide](https://google.github.io/styleguide/cppguide.html).

## Project Instructions and Rubric

Note: regardless of the changes you make, your project must be buildable using
cmake and make!

More information is only accessible by people who are already enrolled in Term 2
of CarND. If you are enrolled, see [the project page](https://classroom.udacity.com/nanodegrees/nd013/parts/40f38239-66b6-46ec-ae68-03afd8a601c8/modules/f1820894-8322-4bb3-81aa-b26b3c6dcbaf/lessons/e8235395-22dd-4b87-88e0-d108c5e5bbf4/concepts/6a4d8d42-6a04-4aa6-b284-1697c0fd6562)
for instructions and the project rubric.

## Hints!

* You don't have to follow this directory structure, but if you do, your work
  will span all of the .cpp files here. Keep an eye out for TODOs.

## Call for IDE Profiles Pull Requests

Help your fellow students!

We decided to create Makefiles with cmake to keep this project as platform
agnostic as possible. Similarly, we omitted IDE profiles in order to we ensure
that students don't feel pressured to use one IDE or another.

However! I'd love to help people get up and running with their IDEs of choice.
If you've created a profile for an IDE that you think other students would
appreciate, we'd love to have you add the requisite profile files and
instructions to ide_profiles/. For example if you wanted to add a VS Code
profile, you'd add:

* /ide_profiles/vscode/.vscode
* /ide_profiles/vscode/README.md

The README should explain what the profile does, how to take advantage of it,
and how to install it.

Frankly, I've never been involved in a project with multiple IDE profiles
before. I believe the best way to handle this would be to keep them out of the
repo root to avoid clutter. My expectation is that most profiles will include
instructions to copy files to a new location to get picked up by the IDE, but
that's just a guess.

One last note here: regardless of the IDE used, every submitted project must
still be compilable with cmake and make./
