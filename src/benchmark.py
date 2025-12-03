import subprocess
import time
import matplotlib.pyplot as plt
import numpy as np
import os

# --- CONFIGURAÇÃO ---
SOURCE_FILE = "main.cpp"
GENERATOR_FILE = "gerador_p1.cpp"
# Detetar executável (Windows vs Linux/Mac)
EXECUTABLE = "proj1" if os.name != 'nt' else "proj1.exe"
GENERATOR_EXEC = "gen" if os.name != 'nt' else "gen.exe"

# ALTERAÇÃO AQUI: 
# 12 instâncias (cumpre o requisito >10)
# Vai de 100 até 1200. É suficiente para mostrar a curva cúbica sem demorar muito.
test_sizes = [100, 200, 300, 400, 500, 600, 700, 800, 900, 1000, 1100, 1200]

PMAX = 1000
NUM_TRIALS = 5  # 5 repetições para garantir boas barras de erro

def compile_code():
    print("A Compilar código...")
    # Se estiveres em Windows e o g++ não for reconhecido, verifica o PATH.
    subprocess.run(["g++", "-std=c++11", "-O3", "-Wall", SOURCE_FILE, "-o", EXECUTABLE], check=True)
    subprocess.run(["g++", "-O3", GENERATOR_FILE, "-o", GENERATOR_EXEC], check=True)
    print("Compilação concluída.\n")

def run_benchmark():
    results = []
    print(f"{'N':<10} | {'N^3':<15} | {'Média (s)':<10} | {'Desvio (s)':<15}")
    print("-" * 60)

    for n in test_sizes:
        times = []
        
        # 1. Gerar Input
        with open("input.txt", "w") as infile:
            subprocess.run([f"./{GENERATOR_EXEC}", str(n), str(PMAX), "123"], stdout=infile, check=True)

        with open("input.txt", "r") as infile:
            input_content = infile.read()

        # 2. Executar Várias Vezes
        for _ in range(NUM_TRIALS):
            start_time = time.time()
            subprocess.run([f"./{EXECUTABLE}"], input=input_content.encode(), stdout=subprocess.DEVNULL, check=True)
            end_time = time.time()
            times.append(end_time - start_time)

        avg_time = np.mean(times)
        std_dev = np.std(times)
        n_cubed = n**3
        
        results.append((n, n_cubed, avg_time, std_dev))
        # Formatação bonita para o terminal
        print(f"{n:<10} | {n_cubed:<15.2e} | {avg_time:.4f}     | {std_dev:.6f}")

    return results

def plot_results(results):
    x_values = [r[1] for r in results] # N^3
    y_values = [r[2] for r in results] # Tempo Médio
    y_errs = [r[3] for r in results]   # Desvio Padrão

    plt.figure(figsize=(10, 6))
    
    # Plot com Barras de Erro
    plt.errorbar(x_values, y_values, yerr=y_errs, fmt='-o', color='blue', 
                 ecolor='red', capsize=4, label='Dados Experimentais')
    
    plt.xlabel("Complexidade Teórica ($N^3$)")
    plt.ylabel("Tempo de Execução (s)")
    plt.grid(True, linestyle='--', alpha=0.5)
    plt.legend()
    
    filename = "grafico_final_v3.png"
    plt.savefig(filename)
    print(f"\nGráfico salvo como '{filename}'")

def generate_latex_table(results):
    print("\n--- Tabela LaTeX para Copiar ---")
    print("\\begin{table}[h!]")
    print("\\centering")
    print("\\begin{tabular}{|c|c|c|c|}")
    print("\\hline")
    print("\\textbf{N} & \\textbf{Complexidade ($N^3$)} & \\textbf{Tempo (s)} & \\textbf{Desvio (s)} \\\\ \\hline")
    for n, n3, avg, std in results:
        n3_sci = "{:.1e}".format(n3).replace("+", "")
        print(f"{n} & ${n3_sci}$ & {avg:.3f} & {std:.3f} \\\\ \\hline")
    print("\\end{tabular}")
    print("\\caption{Tempos de execução (Média de 5 execuções).}")
    print("\\label{tab:tempos}")
    print("\\end{table}")

if __name__ == "__main__":
    try:
        compile_code()
        data = run_benchmark()
        plot_results(data)
        generate_latex_table(data)
        if os.path.exists("input.txt"): os.remove("input.txt")
    except Exception as e:
        print(f"Erro: {e}")