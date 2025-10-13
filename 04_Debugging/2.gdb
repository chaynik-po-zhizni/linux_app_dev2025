set pagination off

set $num = 0 

b 34 if ++$num && ($num >= 28) && ($num <= 35)
command 1
    printf "@@@ %d %d %d %d\n", M, N, S, i
    c
end

run -100 100 3 > /dev/null 
q