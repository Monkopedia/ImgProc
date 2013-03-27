load arg1 image

threshold threshfilt arg3 arg4
filter threshfilt image thresholded
erode erodefilt arg3
filter erodefilt thresholded eroded

save arg2 eroded
