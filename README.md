**AxBench** is a benchmark suite for approximate computing. We develop **AxBench** in C++, aiming to provide a set of representative applications from various domains to explore different aspect of the approximate computing. **AxBench** is developed in Alternative Computing Technologies (ACT) Laboratory, Georgia Institute of Technology.

*** === Papers === ***

We actively work on **AxBench** to add more applications from different domains (e.g. Computer Vision, Data Analytics, Multimedia, Web Search, Finance, etc.). We will also be working on adding different features to this benchmark suite in order to enable researchers to study different aspects of approximate computing.  As a courtesy to the developers, we ask that you please cite our papers from MICRO'12 and ISCA'14 describing the suite:

          1. H. Esmaeilzadeh, A. Sampson, L. Ceze, D. Burger, 
              "Neural acceleration for general-purpose approximate programs", MICRO 2012.

*** === Applications === ***

1. **Black-Scholes [Financial Analysis]**: This application calculates the price of European options. We adapted this benchmark from **Parsec** benchmark suite to use for approximate computing purpose. 
2. **FFT [Signal Processing]**: This application calculates the radix-2 Cooley-Turkey Fast Fourier for a set of random floating point numbers. 
3. **Inversek2j [Robotics]**: This applications find the coordinate of a 2-joint arm given their angle.
4. **Jmeint [3D Gaming]**: This application detects the intersection of two 3D triangles.
5. **JPEG encoder [Compression]**: This application compress an image.
6. **K-means [Machine Learning]**: This application performs K-means clustering on a set of random (r, g, b) values.
7. **Sobel [Image Processing]**: Sobel filter detects the edges of a color image.

*** === Dependencies === ***

**AxBench** was developed on Ubuntu Linux (this release was tested with Ubuntu version 12.04). In principal, **AxBench** should work with any Linux distribution as long as the following software dependencies are satisfied.

1. Boost Libraries [version 1.49 or higher]
2. Python [version 2.7 or higher]
3. G++ [4.6 or higher]
4. Fast Artificial Neural Network Library (FANN) [version 2.2.0 or higher]

*** === Build and Run AxBench ===***

1) After downloading the **AxBench**, please go to the *parrot.c/src* directory and run *bash buildlib.sh*. It will create a static library which will be later used to execute the Parrot transformation on the applications.

2) Then, modify **config.mk** in the **applications** folder with the location of the Parrot and FANN library.

3) You are set to use **AxBench**. You can simply execute the *run.sh* script to make or run each of the applications. 

**AxBench** contains both the applications and the necessary annotations for execution on NPU.
 
*** === Software License === ***

The license is a free non-exclusive, non-transferable license to reproduce, use, modify and display the source code version of the Software, with or without modifications solely for non-commercial research, educational or evaluation purposes. The license does not entitle Licensee to technical support, telephone assistance, enhancements or updates to the Software. All rights, title to and ownership interest in Software, including all intellectual property rights therein shall remain in Georgia Institute of Technology.

*** === Questions === ***

Please forward your questions to: *a.yazdanbakhsh@gatech.edu*

*** === Maintained by === ***

    Amir Yazdanbakhsh ( http://www.cc.gatech.edu/~ayazdanb/ )