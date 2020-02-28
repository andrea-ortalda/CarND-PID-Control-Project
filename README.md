# CarND-Path-Planning-Project

[![Udacity - Self-Driving Car NanoDegree](https://s3.amazonaws.com/udacity-sdc/github/shield-carnd.svg)](http://www.udacity.com/drive)

[//]: # (Image References)

[video0]: ./PID.gif "Final video"

<p align="center">
	<img src="/write_up_images/PID.gif" alt="Video Output"
	title="Video Output"  />
</p>

## Overwiew

Scope of the project is to implement in C++ a PID controller for the steering angle of a self_driving car. The steering wheel value depends on the Cross Track Error (CTE), defined as the displacement error from the center of the line. 
Here the Udacity term 2 simulator is used (4th project).

## Parameters Description

The Proportional component P is acting directly on the current displacement error. An high value will lead to a very agressive adjustement, giving an unstable behavior. On the other hand, a too low value will lead to a softer response, but not enough for letting the car on the track.

The Integral component I is acting on the steady-state behavior and can correct for steering bias, such as a constant mis-orientation of the wheels. During the tuning, it was observed that if it's too high overshoot is experienced.

The Derivative component is acting on the difference between the current error and the previous error. It was observed that when it is too high, we have overshoot and instability. If it's too low, the vehicle will react too slowly and it will drive off the track.

## Tuning

### Manual tuning

PID parameters were firstly tuned manually. Each parameter was tuned with the other 2 set to 0, in order to see single effects of the tunes. This led to the values used for Initialization and starting point for the twiddle algorithm.

### Twiddle

After the manual tuning, twiddle algorithm was performed. This lead to small cheanges in the I and D parameters , while the P parameter was the most affected one. Twidlle algorithm is implemented with a boolean value that enable the tuning, set to false as default.

Parameter | Manual | Twiddle |
| --- | :-------------: | :-------------: |
P | 0.15 | 0.160013 |
I | 0.005 | 0.00500106 |
D | 5 | 5.00106 |

<p align="center">
	<img src="/write_up_images/twiddle.png" alt="Twiddle parameters"
	title="Twiddle parameters"  />
</p>


