load arg1 image

copy image inverted-image
histoeq invert-channel arg3
filter invert-channel image inverted-image

save arg2 inverted-image
