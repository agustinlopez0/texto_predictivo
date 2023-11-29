
import sys
from collections import defaultdict

def cargar_oraciones(nombre_persona):
    f = open(f"Entradas/{nombre_persona}.txt", 'r+')
    oraciones = f.readlines()
    f.close()
    return oraciones

def cargar_frases_incompletas(nombre_persona):
    f = open(f"Frases/{nombre_persona}.txt", 'r+')
    frases_incompletas = f.readlines()
    f.close
    return frases_incompletas

def procesar_palabra(palabra_anterior, palabra_posterior, frecuencias):
    candidatas = defaultdict(int)
    if palabra_anterior:
        for i in frecuencias:
            if i[0] == palabra_anterior:
                candidatas[i[1]] += frecuencias[i]
    if palabra_posterior:
        for i in frecuencias:
            if i[1] == palabra_posterior:
                candidatas[i[0]] += frecuencias[i]
    if candidatas:
        return max(candidatas, key=candidatas.get)
    return "[Ninguna palabra encontrada]"
     

def generar_bigramas(oraciones):
    frecuencias = defaultdict(int)
    for oracion in oraciones:
        oracion = oracion.split()
        for i in range(len(oracion) - 1):
            bigrama = (oracion[i], oracion[i + 1])
            frecuencias[bigrama] += 1
    return frecuencias
    

def completar_frase(frases_incompletas, oraciones):
    frecuencias = generar_bigramas(oraciones)

    frases_completas = []
    for oracion in frases_incompletas:
        oracion = oracion.split()
        for i, palabra in enumerate(oracion):
            if palabra == '_':
                anterior = oracion[i - 1] if i > 0 else ''
                posterior = oracion[i + 1] if i < len(oracion) - 1 else ''
                oracion[i] = procesar_palabra(anterior, posterior, frecuencias)
        frases_completas.append(' '.join(oracion))
    return frases_completas
    
def main():
    if len(sys.argv) < 2:
        print("Uso: python main.py <nombre>")
        return

    nombre_persona = sys.argv[1]

    oraciones = cargar_oraciones(nombre_persona)
    frases_incompletas = cargar_frases_incompletas(nombre_persona)

    frases_completas = completar_frase(frases_incompletas, oraciones)
    
    f = open(f"Salidas/{nombre_persona}.txt", "w")
    for frase in frases_completas:
        f.write(f"{frase}\n")
    
    
        
if __name__ == "__main__":
    main()
