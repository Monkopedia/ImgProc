load arg1 image

copy image inverted-image
threshold invert-channel arg3 arg4
filter invert-channel image inverted-image

save arg2 inverted-image
