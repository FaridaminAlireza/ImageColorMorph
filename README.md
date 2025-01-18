# ImageProcessor

The goal of this project is to create smooth coloring effects for a given image.
Given a digital image, it generates a number of pre-defined transformations on the original image,
and creates video by transitioning between the generated transformations.

## Workflow Overview

1. Create a number of transformations from a given image.
2. Given the list of transformed images, iterate over the  pair of adjacent images and apply
color transition on each pair. This will result in generating a number of images depicting the progression
of the color transition.
3. Create video from the generated images in the previous step.

The list of implemented transformations:
  - Compliment
  - Analogue Color Shift
  - Increase Contrast
  - Border Overlay
  - Black
  - White
  - Gray Scale

  The steps for color transition is as following:
  1. Group the pixel values of the source image into color  buckets, where each bucket represents a well known color.
  2. Order the color buckets based on the euclidean distance of their representative pixel value.
  3. For each ordered bucket, replace the source pixel value with the target pixel value by iterating over the bucket's allocated pixels. After replacing each [N] pixel, create a new image.

## Third-party libraries
OpenCV

## Design Patterns
  - Command pattern
  - Factory pattern
  - Polymorphic execution of function calls
  - Separating third-party library from business logic
    - Dependency Inversion Principle (DISP)
    -  Facade Pattern




## Command Reference Guide

STATS [image_path]
  - Creates a histogram from pixel values, where buckets are a  pre-defined list of well known colors.
  - The histogram is stored in  data/stats.

COLOR_MORPH [image_path]:
  -  Applies color transitioning.
  - The transformations are stored in data/transformations.
  - The intermediate images are stored in data/frames.
  - The video is build in data/video.

## Limitations:
  The current implementation works best on images with low number of pixels as the pixel processing is performed sequentially.
  Also,  the bucket identification is performed independently for each pixel. Applying the well known algorithm [X] that divides the image into boxes based on their second derivative can dramatically enhance the bucketing process.



