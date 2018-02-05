BEGIN{print "SKU,QTY"}NR>1{a[$1] = $2}END{for (i in a) if(i != "SKU")print i","a[i]}
