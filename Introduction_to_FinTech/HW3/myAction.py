import numpy as np

# A simple greedy approach
def myActionSimple(priceMat, transFeeRate):
    # Explanation of my approach:
	# 1. Technical indicator used: Watch next day price
	# 2. if next day price > today price + transFee ==> buy
    #       * buy the best stock
	#    if next day price < today price + transFee ==> sell
    #       * sell if you are holding stock
    # 3. You should sell before buy to get cash each day
    # default
    cash = 1000
    hold = 0
    # user definition
    nextDay = 1
    dataLen, stockCount = priceMat.shape  # day size & stock count   
    stockHolding = np.zeros((dataLen,stockCount))  # Mat of stock holdings
    actionMat = []  # An k-by-4 action matrix which holds k transaction records.
    
    for day in range( 0, dataLen-nextDay ) :
        dayPrices = priceMat[day]  # Today price of each stock
        nextDayPrices = priceMat[ day + nextDay ]  # Next day price of each stock
        
        if day > 0:
            stockHolding[day] = stockHolding[day-1]  # The stock holding from the previous action day
        
        buyStock = -1  # which stock should buy. No action when is -1
        buyPrice = 0  # use how much cash to buy
        sellStock = []  # which stock should sell. No action when is null
        sellPrice = []  # get how much cash from sell
        bestPriceDiff = 0  # difference in today price & next day price of "buy" stock
        stockCurrentPrice = 0  # The current price of "buy" stock
        
        # Check next day price to "sell"
        for stock in range(stockCount) :
            todayPrice = dayPrices[stock]  # Today price
            nextDayPrice = nextDayPrices[stock]  # Next day price
            holding = stockHolding[day][stock]  # how much stock you are holding
            
            if holding > 0 :  # "sell" only when you have stock holding
                if nextDayPrice < todayPrice*(1+transFeeRate) :  # next day price < today price, should "sell"
                    sellStock.append(stock)
                    # "Sell"
                    sellPrice.append(holding * todayPrice)
                    cash = holding * todayPrice*(1-transFeeRate) # Sell stock to have cash
                    stockHolding[day][sellStock] = 0
        
        # Check next day price to "buy"
        if cash > 0 :  # "buy" only when you have cash
            for stock in range(stockCount) :
                todayPrice = dayPrices[stock]  # Today price
                nextDayPrice = nextDayPrices[stock]  # Next day price
                
                if nextDayPrice > todayPrice*(1+transFeeRate) :  # next day price > today price, should "buy"
                    diff = nextDayPrice - todayPrice*(1+transFeeRate)
                    if diff > bestPriceDiff :  # this stock is better
                        bestPriceDiff = diff
                        buyStock = stock
                        stockCurrentPrice = todayPrice
            # "Buy" the best stock
            if buyStock >= 0 :
                buyPrice = cash
                stockHolding[day][buyStock] = cash*(1-transFeeRate) / stockCurrentPrice # Buy stock using cash
                cash = 0
                
        # Save your action this day
        if buyStock >= 0 or len(sellStock) > 0 :
            action = []
            if len(sellStock) > 0 :
                for i in range( len(sellStock) ) :
                    action = [day, sellStock[i], -1, sellPrice[i]]
                    actionMat.append( action )
            if buyStock >= 0 :
                action = [day, -1, buyStock, buyPrice]
                actionMat.append( action )
    #print(actionMat)
    return actionMat

# A DP-based approach to obtain the optimal return
def myAction01(priceMat, transFeeRate):
    actionMat = []  # An k-by-4 action matrix which holds k transaction records.
    cash = 1000
    # user definition
    totalDays, stockCount = priceMat.shape  # day size & stock count   
    stockHolding = np.zeros((totalDays, stockCount))  # Mat of stock holdings
    cashHolding = [0] * totalDays
    pre_step = -1   # -1 for cash, number for stock

    cashHolding[0] = cash

    for i in range(stockCount):
        stockHolding[0][i] = cash*(1-transFeeRate)/priceMat[0][i]

    # DP
    for i in range(1, totalDays):
        cashHolding[i] = cashHolding[i-1]
        for j in range(stockCount):
            if cashHolding[i] < stockHolding[i-1][j] * priceMat[i][j] * (1-transFeeRate):
                cashHolding[i] = stockHolding[i-1][j] * priceMat[i][j] * (1-transFeeRate)
        for j in range(stockCount):
            stockHolding[i][j] = max(stockHolding[i-1][j], cashHolding[i-1]*(1-transFeeRate)/priceMat[i][j], cashHolding[i]*(1-transFeeRate)/priceMat[i][j])
    
    # back trace
    for i in range(totalDays-1, 0, -1):
        if pre_step == -1:
            if cashHolding[i] == cashHolding[i-1]:
                continue
            for j in range(stockCount):
                if cashHolding[i] == stockHolding[i-1][j] * priceMat[i][j] * (1-transFeeRate):
                    action = [i, j, -1, stockHolding[i-1][j] * priceMat[i][j]]
                    pre_step = j
                    actionMat.append( action )
                    break

        else:
            if stockHolding[i][pre_step] == cashHolding[i-1] * (1-transFeeRate) / priceMat[i][j] :
                action = [i, -1, pre_step, cashHolding[i-1]]
                pre_step = -1
                actionMat.append( action )
            elif stockHolding[i][pre_step] == cashHolding[i] * (1-transFeeRate) / priceMat[i][j] :
                for j in range(stockCount):
                    if cashHolding[i] == stockHolding[i-1][j] * priceMat[i][j] * (1-transFeeRate):
                        action = [i, j, pre_step, stockHolding[i-1][j] * priceMat[i][j]]
                        pre_step = j
                        actionMat.append( action )
                        break
                
            else:
                continue
        
    
    if pre_step != -1:
        action = [0, -1, pre_step, cash]
        actionMat.append( action )

    actionMat.reverse()
    return actionMat

# An approach that allow non-consecutive K days to hold all cash without any stocks
def myAction02(priceMat, transFeeRate, K):
    actionMat = []  # An k-by-4 action matrix which holds k transaction records.
    cash = 1000
    # user definition
    totalDays, stockCount = priceMat.shape  # day size & stock count   
    stockHolding = np.zeros((totalDays, stockCount))  # Mat of stock holdings
    cashHolding = [0] * totalDays
    pre_step = -1   # -1 for cash, number for stock

    cashHolding[0] = cash

    for i in range(stockCount):
        stockHolding[0][i] = cash*(1-transFeeRate)/priceMat[0][i]
        
    # DP
    for i in range(1, totalDays):
        cashHolding[i] = cashHolding[i-1]
        for j in range(stockCount):
            if cashHolding[i] < stockHolding[i-1][j] * priceMat[i][j] * (1-transFeeRate):
                cashHolding[i] = stockHolding[i-1][j] * priceMat[i][j] * (1-transFeeRate)
        for j in range(stockCount):
            stockHolding[i][j] = max(stockHolding[i-1][j], cashHolding[i-1]*(1-transFeeRate)/priceMat[i][j], cashHolding[i]*(1-transFeeRate)/priceMat[i][j])
    
    # back trace
    keep_cash_days = 0
    pre_step = -1
    for i in range(totalDays-1, 0, -1):
        if pre_step == -1:
            keep_cash_days +=1
            if cashHolding[i] == cashHolding[i-1]:
                continue
            for j in range(stockCount):
                if cashHolding[i] == stockHolding[i-1][j] * priceMat[i][j] * (1-transFeeRate):
                    pre_step = j
                    break

        else:
            if stockHolding[i][pre_step] == cashHolding[i-1] * (1-transFeeRate) / priceMat[i][j] :
                pre_step = -1 
            elif stockHolding[i][pre_step] == cashHolding[i] * (1-transFeeRate) / priceMat[i][j] :
                for j in range(stockCount):
                    if cashHolding[i] == stockHolding[i-1][j] * priceMat[i][j] * (1-transFeeRate):
                        pre_step = j
                        break
            else:
                continue

    #decide when to keep cash
    earning_rate_list = []
    
    for i in range(1, totalDays-1):
        earning_rate_list.append([i, (cashHolding[i+1] - cashHolding[i])/cashHolding[i]])
    full_earning_rate_list = sorted(earning_rate_list, key = lambda x:x[1])
    list_end_idx = int((K-keep_cash_days)/2)

    while keep_cash_days < K:
        #print(keep_cash_days)
        earning_rate_list = full_earning_rate_list[0 : list_end_idx]
        
        #DP again
        idx_cnt = 0
        for i in range(1, totalDays):
            if i == earning_rate_list[idx_cnt][0]:
                if idx_cnt+1 < len(earning_rate_list):
                    idx_cnt += 1
                cashHolding[i] = cashHolding[i-1]
                for j in range(stockCount):
                    stockHolding[i][j] = 0
                    if cashHolding[i] < stockHolding[i-1][j] * priceMat[i][j] * (1-transFeeRate):
                        cashHolding[i] = stockHolding[i-1][j] * priceMat[i][j] * (1-transFeeRate)
                
            elif i == earning_rate_list[idx_cnt-1][0] + 1:
                cashHolding[i] = cashHolding[i-1]
                for j in range(stockCount):
                    stockHolding[i][j] = cashHolding[i-1]*(1-transFeeRate)/priceMat[i][j]

            else:
                cashHolding[i] = cashHolding[i-1]
                for j in range(stockCount):
                    if cashHolding[i] < stockHolding[i-1][j] * priceMat[i][j] * (1-transFeeRate):
                        cashHolding[i] = stockHolding[i-1][j] * priceMat[i][j] * (1-transFeeRate)
                for j in range(stockCount):
                    stockHolding[i][j] = max(stockHolding[i-1][j], cashHolding[i-1]*(1-transFeeRate)/priceMat[i][j], cashHolding[i]*(1-transFeeRate)/priceMat[i][j])

        # back trace
        keep_cash_days = 0
        pre_step = -1
        for i in range(totalDays-1, 0, -1):
            if pre_step == -1:
                keep_cash_days +=1
                #print("keep at :", i)
                if cashHolding[i] == cashHolding[i-1]:
                    continue
                for j in range(stockCount):
                    if cashHolding[i] == stockHolding[i-1][j] * priceMat[i][j] * (1-transFeeRate):
                        pre_step = j
                        break

            else:
                if stockHolding[i][pre_step] == cashHolding[i-1] * (1-transFeeRate) / priceMat[i][j] :
                    pre_step = -1 
                elif stockHolding[i][pre_step] == cashHolding[i] * (1-transFeeRate) / priceMat[i][j] :
                    for j in range(stockCount):
                        if cashHolding[i] == stockHolding[i-1][j] * priceMat[i][j] * (1-transFeeRate):
                            pre_step = j
                            break
                else:
                    continue

        list_end_idx += (K-keep_cash_days)/2 + 1
        list_end_idx = int(list_end_idx)
        if list_end_idx > len(full_earning_rate_list):
            #print("Not found")
            break

    # back trace
    keep_cash_days = 0
    pre_step = -1
    for i in range(totalDays-1, 0, -1):
        if pre_step == -1:
            keep_cash_days +=1
            if cashHolding[i] == cashHolding[i-1]:
                continue
            for j in range(stockCount):
                if cashHolding[i] == stockHolding[i-1][j] * priceMat[i][j] * (1-transFeeRate):
                    pre_step = j
                    action = [i, pre_step, -1, stockHolding[i-1][j] * priceMat[i][j]]
                    actionMat.append( action )
                    break

        else:
            if stockHolding[i][pre_step] == cashHolding[i-1] * (1-transFeeRate) / priceMat[i][j] :
                pre_step = -1 
                action = [i, pre_step, j, cashHolding[i-1]]
                actionMat.append( action )
            elif stockHolding[i][pre_step] == cashHolding[i] * (1-transFeeRate) / priceMat[i][j] :
                for j in range(stockCount):
                    if cashHolding[i] == stockHolding[i-1][j] * priceMat[i][j] * (1-transFeeRate):
                        action = [i, j, pre_step, stockHolding[i-1][j] * priceMat[i][j]]
                        pre_step = j
                        actionMat.append( action )
                        break
            else:
                continue
       

    if pre_step != -1:
        action = [0, -1, pre_step, cash]
        actionMat.append( action )
    #print(keep_cash_days)
    actionMat.reverse()
    return actionMat


# An approach that allow consecutive K days to hold all cash without any stocks    
def myAction03(priceMat, transFeeRate, K):
    actionMat = []  # An k-by-4 action matrix which holds k transaction records.
    cash = 1000
    # user definition
    totalDays, stockCount = priceMat.shape  # day size & stock count   
    stockHolding = np.zeros((totalDays, stockCount))  # Mat of stock holdings
    cashHolding = [0] * totalDays
    pre_step = -1   # -1 for cash, number for stock

    cashHolding[0] = cash

    for i in range(stockCount):
        stockHolding[0][i] = cash*(1-transFeeRate)/priceMat[0][i]
        
    # DP
    for i in range(1, totalDays):
        cashHolding[i] = cashHolding[i-1]
        for j in range(stockCount):
            if cashHolding[i] < stockHolding[i-1][j] * priceMat[i][j] * (1-transFeeRate):
                cashHolding[i] = stockHolding[i-1][j] * priceMat[i][j] * (1-transFeeRate)
        for j in range(stockCount):
            stockHolding[i][j] = max(stockHolding[i-1][j], cashHolding[i-1]*(1-transFeeRate)/priceMat[i][j], cashHolding[i]*(1-transFeeRate)/priceMat[i][j])
    
    #decide when to keep cash
    min_earning_rate = (cashHolding[K] - cashHolding[0])/cashHolding[0]
    for i in range(K, totalDays):
        if (cashHolding[i] - cashHolding[i-K])/cashHolding[i-K] < min_earning_rate:
            min_start_idx = i-K
            min_earning_rate = (cashHolding[i] - cashHolding[i-K])/cashHolding[i-K]
    
    # DP with keeping cash consecutively
    for i in range(1, totalDays):
        if i < min_start_idx or i > min_start_idx + K:
            cashHolding[i] = cashHolding[i-1]
            for j in range(stockCount):
                if cashHolding[i] < stockHolding[i-1][j] * priceMat[i][j] * (1-transFeeRate):
                    cashHolding[i] = stockHolding[i-1][j] * priceMat[i][j] * (1-transFeeRate)
            for j in range(stockCount):
                stockHolding[i][j] = max(stockHolding[i-1][j], cashHolding[i-1]*(1-transFeeRate)/priceMat[i][j], cashHolding[i]*(1-transFeeRate)/priceMat[i][j])
    
        elif i == min_start_idx:
            cashHolding[i] = cashHolding[i-1]
            for j in range(stockCount):
                stockHolding[i][j] = 0
                if cashHolding[i] < stockHolding[i-1][j] * priceMat[i][j] * (1-transFeeRate):
                    cashHolding[i] = stockHolding[i-1][j] * priceMat[i][j] * (1-transFeeRate)

        elif i == min_start_idx + K:
            cashHolding[i] = cashHolding[i-1]
            for j in range(stockCount):
                stockHolding[i][j] = cashHolding[i-1]*(1-transFeeRate)/priceMat[i][j]
        else:
            cashHolding[i] = cashHolding[i-1]
            for j in range(stockCount):
                stockHolding[i][j] = 0
    
    # back trace
    for i in range(totalDays-1, 0, -1):
        if pre_step == -1:
            if cashHolding[i] == cashHolding[i-1]:
                continue
            for j in range(stockCount):
                if cashHolding[i] == stockHolding[i-1][j] * priceMat[i][j] * (1-transFeeRate):
                    action = [i, j, -1, stockHolding[i-1][j] * priceMat[i][j]]
                    pre_step = j
                    actionMat.append( action )
                    break

        else:
            if stockHolding[i][pre_step] == cashHolding[i-1] * (1-transFeeRate) / priceMat[i][j] :
                action = [i, -1, pre_step, cashHolding[i-1]]
                pre_step = -1
                actionMat.append( action )
            elif stockHolding[i][pre_step] == cashHolding[i] * (1-transFeeRate) / priceMat[i][j] :
                for j in range(stockCount):
                    if cashHolding[i] == stockHolding[i-1][j] * priceMat[i][j] * (1-transFeeRate):
                        action = [i, j, pre_step, stockHolding[i-1][j] * priceMat[i][j]]
                        pre_step = j
                        actionMat.append( action )
                        break
                
            else:
                continue
       

    if pre_step != -1:
        action = [0, -1, pre_step, cash]
        actionMat.append( action )

    actionMat.reverse()
    return actionMat
