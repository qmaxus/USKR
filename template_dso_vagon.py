loc= ['500400', '004005', '004002', '500405','004055', '504005','550400','00040005','50004000']
locset = set(loc)

type_vagons={2:[],
             1:['5004005','5004002','500040005','500040002',],
             0:['20402']}
template_dataframe = [ "indicator",
                          "time",
                            "condition",
                            "axle",
                            "time_min",
                            "time_max",
                            "direction",
                            "probility",
                            "SendCamera",
                            "type"]