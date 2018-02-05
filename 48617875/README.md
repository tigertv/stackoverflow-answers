# [Question](https://stackoverflow.com/questions/48617875/add-empty-line-between-each-time-blocks)
I have the input log a form:
```
system 2018-02-05 04:15:49 :: aaaaaaaaaaaaa  
system 2018-02-05 04:15:51 :: aaaaaaaaaaaaa  
system 2018-02-05 04:15:51 :: aaaaaaaaaaaaa  
system 2018-02-05 04:15:52 :: aaaaaaaaaaaaa  
system 2018-02-05 04:15:53 :: aaaaaaaaaaaaa  
system 2018-02-05 04:20:06 :: ccccccccccccc
system 2018-02-05 04:21:10 :: bbbbbbbbbbbbb
system 2018-02-05 04:21:10 :: ccccccccccccc
system 2018-02-05 04:21:10 :: ccccccccccccc
system 2018-02-05 04:21:10 :: ccccccccccccc
system 2018-02-05 04:23:49 :: bbbbbbbbbbbbb
system 2018-02-05 04:23:49 :: ccccccccccccc
```
and want to have separated each time block by empty line. Expected output for above input would be:
```
system 2018-02-05 04:15:49 :: aaaaaaaaaaaaa

system 2018-02-05 04:15:51 :: aaaaaaaaaaaaa  
system 2018-02-05 04:15:51 :: aaaaaaaaaaaaa  

system 2018-02-05 04:15:52 :: aaaaaaaaaaaaa  

system 2018-02-05 04:15:53 :: aaaaaaaaaaaaa  

system 2018-02-05 04:20:06 :: ccccccccccccc

system 2018-02-05 04:21:10 :: bbbbbbbbbbbbb
system 2018-02-05 04:21:10 :: ccccccccccccc
system 2018-02-05 04:21:10 :: ccccccccccccc
system 2018-02-05 04:21:10 :: ccccccccccccc

system 2018-02-05 04:23:49 :: bbbbbbbbbbbbb
system 2018-02-05 04:23:49 :: ccccccccccccc
```

# [Answer](https://stackoverflow.com/a/48629416/9210255)

Another one awk approach
```bash
awk -F: '$3!=p{print ""}{p=$3}{print}' file
```
We choose as separator symbol ':', and to define when we have to print a space line we check seconds, in this case it's $3.
