I have a csv file that looks like this:

SKU,QTY

KA006-001,2  
KA006-001,33  
KA006-001,46  
KA009-001,22  
KA009-001,7  
KA010-001,18  
KA014-001,3  
KA014-001,42  
KA015-001,1  
KA015-001,16  
KA020-001,6  
KA022-001,56  
The first column is SKU. The second column is QTY number.

Some lines in (SKU column only) are identical.

I need to achieve the following:

SKU,QTY  
KA006-001,81 (2+33+46)  
KA009-001,29 (22+7)  
KA010-001,18  
KA014-001,45 (3+42)  
so on...

I tried different things , loop statements and arrays. Got so lost, got headache.

My code:

#!/bin/bash

while IFS=, read sku qty
do
    echo "SKU='$sku' QTY='$qty'"
    if [ "$sku" = "$sku" ]
    then
        #x=("$sku" != "$sku")
        for i in {0..3}; do echo $sku[$i]=$qty; done
    fi

done < 2asg.csv
