def cut_info(arr, ar_cam, ar_coupl,memory_number_frame_annot, number_event, send_dso_number, time_cut_send, lasers_arr,account_axle):
    """
       CUT INFO
       param arr list: dso list
       param ar_cam list: number list
       param ar_coupl list: coupl list
       param memory_number_frame_annot list: memory frame list
       return list: unified list of DSO and recognized objects
    """
    end_coupl = None
    quant_couple = len(arr)
    log_utils.logstat(f" ---------{time_cut_send}--  : { quant_couple}---{number_event}----------", "graf", DEBUG=False)
    flag = True
    i=0

    y_max_memory = time_cut_send if time_cut_send is not None else 0
    one_key  =True

    for key_, x in enumerate(arr):

        y_min =y_max_memory
        y_max = get_max_time(arr,key_, quant_couple)
        y_max_memory = y_max

        ar_cam, new_number = checkio(ar_cam, y_min, y_max)
        ar_coupl, new_coupl = checkio(ar_coupl, convert_time(x['StartDate']),y_max)
        lasers_coupl =[laser for laser in lasers_arr if x['StartDate']<laser['c']['time']<y_max]

        log_utils.logstat(f"    **num time   : {y_min}  {y_max}", "graf", DEBUG=False)
        log_utils.logstat(f"    **coupl time : { convert_time(x['StartDate'])} {y_max}", "graf", DEBUG=False)
        log_utils.logstat(f"    --num        : { new_number}", "graf", DEBUG=False)
        log_utils.logstat(f"    --coupl      : { new_coupl}", "graf", DEBUG=False)
        log_utils.logstat(f"    --CountAxis : {x['CountAxis']}", "graf", DEBUG=False)
        log_utils.logstat(f"    --Reliability : {x['Reliability']}", "graf", DEBUG=False)
        log_utils.logstat(f"    --Dir : {x['Dir']}", "graf", DEBUG=False)
        log_utils.logstat(f"    --send_dso_number : {send_dso_number}", "graf", DEBUG=False)
        log_utils.logstat(f"    --laser  : {[(lsr['c']['msg'],lsr['c']['time']) for lsr in lasers_coupl]}", "graf", DEBUG=False)
        log_utils.logstat(f"    --account_axle : {account_axle}", "graf", DEBUG=False)
        log_utils.logstat(f"    --IsLastVagon : {x['IsLastVagon']}", "graf", DEBUG=False)


        x['num'] =  new_number
        x['coupl'] = new_coupl
        x['flag'] = False
        x['status_vgn']= 0
        x['num_axis']= 0

        x['AxisList'] = [
            (
                'a', i['NumAxis'], mean([convert_time(v['Time']) for v in i['ListEventsDso']]),
                i['DistanceAxisLeft']['TypeDistanceAxis'],
                i['DistanceAxisRight']['TypeDistanceAxis'],
                [convert_time(v['Time']) for v in i['ListEventsDso']],
                max(set([s['Direction'] for s in i['ListEventsDso']]), key = lambda x: [s['Direction'] for s in
                                                                                            i['ListEventsDso']].count(x))
            ) for i in x['AxisList']

           if abs(sum([ca['Direction'] if ca['Direction']!=0 else -1 for ca in i['ListEventsDso']])) > 1
        ]


        if x['Reliability'] !=666 or ( x['Reliability'] ==666 and x['CountAxis']>5):
            if x['IsLastVagon'] or end_coupl is not None:
               current_time = convert_time(x['EndDate'])
               if end_coupl is not None:
                   end_coupl =current_time if current_time>end_coupl else end_coupl
               else:
                   end_coupl =current_time


            if x['Reliability']==0  or ((len(new_coupl) > 0 or x['Reliability']==1) and sum([f[1] for f in new_number])== len(x['AxisList'])):
                if x['Dir']==2:
                    try:
                        dir_search = [t[6] for t in x['AxisList']]
                        x['Dir']  = max(set(dir_search), key=lambda b: dir_search.count(b),default=2)
                    except Exception as e :
                        log_utils.logstat(f"  err : {e}", "err", DEBUG=False)

                if len(x['AxisList'])>0 and len(send_dso_number)>0:
                    max_send_dso = max(send_dso_number , key=lambda x:x[0], default=(-1,-1))
                    min_rest_dso = min(x['AxisList'], key=lambda p: p[1], default=(-1,-1))
                    if max_send_dso[0]>= min_rest_dso[1]:
                        log_utils.logstat(  f"  { max_send_dso}{ min_rest_dso[1]} ",    "graf", DEBUG=False)
                        flag =False
                rest_axle_vagons = True if (account_axle-x['CountAxis'])>0 or  end_coupl is not None else False

                if (len(x['AxisList']) == x['CountAxis'] or x['Reliability']==0) and len(x['AxisList']) > 0 and key_ == i and flag and rest_axle_vagons:
                    i+=1
                    if (len(x['AxisList']) != x['CountAxis'] and x['Reliability']==0):
                        log_utils.logstat(f"         >>>>>>>> notAxle       : { len(x['AxisList'])} { x['CountAxis']}", "graf", DEBUG=False)
                    coupl_plus = 1 if len(new_coupl) > 0 else 0
                    number_plus = 1 if sum([f[1] for f in new_number]) == len(x['AxisList']) else 0
                    x['status_vgn'] = coupl_plus + number_plus
                    x['flag'] = True
                    account_axle = account_axle-x['CountAxis']
                    if len(x['AxisList'])>0:
                        w = max(x['AxisList'], key=lambda p: p[1])
                        x['num_axis'] = (max(w[5]),w[1] )
                        if check_availabil(send_dso_number,x['num_axis']):
                            log_utils.logstat(f"h: {x['CountAxis']} {x}", "move_send_graf", DEBUG=False)
                            log_utils.logstat(f"           move {x['CountAxis']}", "graf", DEBUG=False)

                elif  len(x['AxisList'])==0:
                    i+=1
            elif  len(x['AxisList'])==0 and one_key:
                one_key = False
                i+=1



    #arr = [b for b in arr  if len(b['AxisList'])>0]

    return arr, end_coupl, account_axle

