set pagination off

b 34 if i % 5 == 0
command 1
    printf "@@@ %d %d %d %d\n", M, N, S, i
    c
end

run 1 12 > /dev/null 
q