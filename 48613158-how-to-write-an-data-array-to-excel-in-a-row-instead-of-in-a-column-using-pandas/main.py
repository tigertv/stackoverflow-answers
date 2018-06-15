#!/usr/bin/python
import pandas as pd 
import xlsxwriter as xlsw

a= [1,2,3,4,5]
df=pd.DataFrame(a)
df = df.transpose()
xlsfile = 'pandas_simple.xlsx'
writer = pd.ExcelWriter(xlsfile, engine='xlsxwriter')

df.to_excel(writer, sheet_name="Sheet1Name",startrow=1, startcol=1, header=False, index=False)
