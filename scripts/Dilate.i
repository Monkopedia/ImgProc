load arg1 image

threshold threshfilt arg3 arg4
filter threshfilt image thresholded
dilate dilatefilt arg3
filter dilatefilt thresholded dilated

save arg2 dilated
