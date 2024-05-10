from csv import reader
import pandas as pd

tempos = []
correntes = []

# Abrir o arquivo CSV e ler os dados
with open('corrente.csv', 'r') as arquivo:
    csv_reader = reader(arquivo)
    next(csv_reader)  # Ignorar o cabeçalho
    for linha in csv_reader:
        tempos.append(float(linha[0]))
        correntes.append(float(linha[1]))

# Calcular a média da corrente
media_corrente = sum(correntes) / len(correntes)

# Imprimir a média da corrente
print("Média da corrente:", media_corrente)
