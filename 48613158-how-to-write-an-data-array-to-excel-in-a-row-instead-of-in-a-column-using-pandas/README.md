# [Question](https://stackoverflow.com/questions/48613158/how-to-write-an-data-array-to-excel-in-a-row-instead-of-in-a-column-using-pandas)

```python
a= [1,2,3,4,5]
df=DataFrame(a)
```
.... #setup excelwriter and dataframe
```python
df.to_excel(writer, sheet_name=sheetname,startrow=1, startcol=1, header=False, index=False)
```
Output:
```
1\n
2\n
3\n
4\n
5
```
How can I get output as:
```
1    2    3    4    5
```
# [Answer](https://stackoverflow.com/a/48613390/9210255)
```python
#!/usr/bin/python
import pandas as pd 
import xlsxwriter as xlsw

a = [1,2,3,4,5]
df = pd.DataFrame(a)
df = df.transpose()
xlsfile = 'pandas_simple.xlsx'
writer = pd.ExcelWriter(xlsfile, engine='xlsxwriter')

df.to_excel(writer, sheet_name="Sheet1Name",startrow=1, startcol=1, header=False, index=False)
```

