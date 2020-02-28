# CarND-Path-Planning-Project

[![Udacity - Self-Driving Car NanoDegree](https://s3.amazonaws.com/udacity-sdc/github/shield-carnd.svg)](http://www.udacity.com/drive)

[//]: # (Image References)

[video0]: ./PID.gif "Final video"

<p align="center">
	<img src="/write_up_images/PID.gif" alt="Video Output"
	title="Video Output"  />
</p>


## Overwiew

In this project a PID controller is implemented to control the steering angle of the self-driving car depending on the current displacement error from the center of the lane (CTE or Cross-Track error), with the Udacity driving simulator. On the other side the vehicle speed is set constant.

## Parameters Description

The Proportional component is acting directly on the current displacement error. During the tuning, it was observed that if it's too high the vehicle will correct too harshly and we will have an unstable response. On the other side, if it's too low the settling time will be to low and the vehicle will go out of the track.

The Integral component is acting on the steady-state behavior and can correct for steering bias, such as a constant mis-orientation of the wheels. During the tuning, it was observed that if it's too high we will have overshoot.

The Derivative component is acting on the difference between the current error and the previous error. It was observed that when it is too high, we have overshoot and instability. If it's too low, the vehicle will react too slowly and it will drive off the track.

## Tuning

### Manual tuning

PID parameters were firstly tuned manually 

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


