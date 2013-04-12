load arg1 image

copy image thresholded-image
threshold threshold-channel blue arg3
filter threshold-channel image thresholded-image
threshold redRemover red 67000
threshold greenRemover green 67000
filter redRemover thresholded-image ti
filter greenRemover ti ti
threshold grayMaker gray 10
filter grayMaker ti ti

dilate dilateFilter gray
filter dilateFilter ti dilated
erode erodeFilter gray
filter erodeFilter dilated ti

save arg2 ti
