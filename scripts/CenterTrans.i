load arg1 image

deftrans n 0 -141 -212
deftrans r 45 0 0
deftrans b 0 141 212
combinetrans nr r n
combinetrans all b nr
transform all image inverted-image

save arg2 inverted-image
