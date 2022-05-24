# whycode-gen
whycode-gen is a tool to generate the [WhyCode/WhyCon markers](https://github.com/jiriUlr/whycon-ros).
To compile it, just type `make`.
Then, it can generate the markers inside the current working directory.
Simply run the binary followed by a number of ID bits or any other options as
```
$ ./whycode_gen -h
Usage: whycon-id-gen [-v] -l
       whycon-id-gen [-v] [-d <distance>] <bits>

    -h     Help
    -v     Verbose while generating canvas
    -l     Generating the original WhyCon marker
    -d     Set minimal Hamming distance (default: 1)
    -c     Draw cross at the marker's centre
    -t     Tight fit the marker without the white border
```

## License
This project is only available for a non-profit, academic, or educational purpose, in any other case or if not sure, please contact us on e-mail [Tomáš Krajník](mailto:tomas.krajnik@fel.cvut.cz) or [Jiří Ulrich](mailto:jiri.ulrich@fel.cvut.cz).
See `LICENSE.md` for details.

## References
1. J. Ulrich, A. Alsayed et al.: <b>Towards fast fiducial marker with full 6 DOF pose estimation</b>. Symposium on Applied Computing, 2022.
1. T. Krajník, M. Nitsche et al.: <b>A Practical Multirobot Localization System</b>. Journal of Intelligent and Robotic Systems (JINT), 2014.
1. P. Lightbody, T. Krajník et al.: <b>An Efficient Visual Fiducial Localisation System</b>. Applied Computing Review, 2017.
1. T. Krajník, M. Nitsche et al.: <b>External localization system for mobile robotics</b>. International Conference on Advanced Robotics (ICAR), 2013.
1. J. Faigl, T. Krajník et al.: <b>Low-cost embedded system for relative localization in robotic swarms</b>. International Conference on Robotics and Automation (ICRA), 2013.
1. M. Nitsche, T. Krajník et al.: <b>WhyCon: An Efficent, Marker-based Localization System</b>. IROS Workshop on Open Source Aerial Robotics, 2015.

## Acknowledgements

The development of this work is currently supported by the Czech Science Foundation project 17-27006Y _STRoLL_.
In the past, the work was supported by EU within its Seventh Framework Programme project ICT-600623 _STRANDS_.
The Czech Republic and Argentina have given support through projects 7AMB12AR022, ARC/11/11 and 13-18316P.
