from csv import reader
import pandas as pd
import plotly.express as px

tem = []
cor = []

with open('corrente.csv', 'r') as arq:
    altitude = reader(arq)
    it = 0
    for a in altitude:
        if not it:
            it += 1
            continue
        tem.append(float(a[0]))
        cor.append(float(a[1]))

dados = pd.DataFrame({
 'tempo': tem,
 'corrente': cor,

})

grafico = px.line(dados, x='tempo', y='corrente', title='Gráfico Corrente X Tempo')

grafico.show()