![Logo](doc/restore-logo.png)

[![Build Status](https://travis-ci.org/NewProggie/Restore.svg?branch=master)](https://travis-ci.org/NewProggie/Restore) [![Coverage Status](https://coveralls.io/repos/NewProggie/Restore/badge.svg?branch=master)](https://coveralls.io/r/NewProggie/Restore?branch=master) [![License](https://img.shields.io/badge/license-MIT-blue.svg)](http://opensource.org/licenses/MIT)

# What is RESTORE?
RESTORE: REconStruction with Texture-based Object REfinement

The 3D reconstruction of real objects from multiple images is a recent and
exciting research topic in the field of computer vision. At the same time the
demand for 3D models in the movie and game industry is increasing.

A simple and cost-efficient way of reconstruction real objects is using only
a single camera. In this project a hard- and software system is developed which
makes the 3D reconstruction of real objects easy. The hardware consists of a
turntable on which an object can be placed, a motor which drives the rotary
table, a controller to control the motor as well as a calibrated camera for
taking pictures of the object.

From the camera images a visual hull is first computed, which gives a first
rough estimation of the 3D reconstruction. This model is then refined using
only the texture of the object.

## License
RESTORE is released under the terms of the MIT License.
