load images/kite.jpg kite                                                                                                                                                     
threshold thresh_filt blue arg1
filter thresh_filt kite red_kite                                                                                                                                                                                    
threshold blue_filt red 67000                                                                                                                                                                      
threshold green_filt green 67000                                                                                                                                                            
filter green_filt red_kite red_kite
filter blue_filt red_kite red_kite
threshold gray_filt gray 100
filter gray_filt red_kite gray_kite
invert inverter gray
filter inverter gray_kite gray_kite
save images/gray_kite.png gray_kite

