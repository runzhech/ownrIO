#-------------------------------------------------------------------------------
# Rolling quantile
def roll_quantile(ndarray[float64_t, cast=True] input, int win,
                  int minp, double quantile):
    '''
    O(N log(window)) implementation using skip list
    '''
    cdef double val, prev, midpoint
    cdef IndexableSkiplist skiplist
    cdef Py_ssize_t nobs = 0, i
    cdef Py_ssize_t N = len(input)
    cdef ndarray[double_t] output = np.empty(N, dtype=float)
    print "Roll Quantile"
    skiplist = IndexableSkiplist(win)

    minp = _check_minp(win, minp, N)

    for i from 0 <= i < minp - 1:
        val = input[i]

        # Not NaN
        nobs += 1
        skiplist.insert(val)

        output[i] = NaN

    for i from minp - 1 <= i < N:
        val = input[i]

        if i > win - 1:
            prev = input[i - win]
            skiplist.remove(prev)
            nobs -= 1

        nobs += 1
        skiplist.insert(val)

        if nobs >= minp:
            idx = int((quantile / 1.) * (nobs - 1))
            output[i] = skiplist.get(idx)
        else:
            output[i] = NaN

    return output


#-------------------------------------------------------------------------------
# Rolling mean

def roll_mean(ndarray[double_t] input,
               int win, int minp):
    cdef:
        double val, prev, result, sum_x = 0
        Py_ssize_t nobs = 0, i, neg_ct = 0
        Py_ssize_t N = len(input)

    cdef ndarray[double_t] output = np.empty(N, dtype=float)
    minp = _check_minp(win, minp, N)

    for i from 0 <= i < minp - 1:
        val = input[i]

        # Not NaN
        nobs += 1
        sum_x += val
        output[i] = NaN

    for i from minp - 1 <= i < N:
        val = input[i]

        nobs += 1
        sum_x += val

        if i > win - 1:
            prev = input[i - win]
            sum_x -= prev
            nobs -= 1

        if nobs >= minp:
            result = sum_x / nobs
            output[i] = result
        else:
            output[i] = NaN

    return output