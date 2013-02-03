load arg1 image

deftrans n 0 -512 -640
deftrans r 180 0 0
deftrans b 0 512 640
combinetrans nr r n
combinetrans all b nr
transform all image inverted-image

save arg2 inverted-image
