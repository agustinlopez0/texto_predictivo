import sys
from collections import defaultdict

# Devuelve una lista con las lineas del archivo Entradas/<nombre_persona>.txt
def cargar_oraciones(nombre_persona):
    f = open(f"Entradas/{nombre_persona}.txt", 'r+')
    oraciones = f.readlines()
    f.close()
    return oraciones

# Devuelve una lista con las lineas del archivo Frases/<nombre_persona>.txt
def cargar_frases_incompletas(nombre_persona):
    f = open(f"Frases/{nombre_persona}.txt", 'r+')
    frases_incompletas = f.readlines()
    f.close
    return frases_incompletas

# Dado un string genera un diccionario donde las claves son tuplas de dos palabras consecutivas en el string
#   y los valores son la cantidad de apariciones que tiene cada una
def generar_bigramas(oraciones):
    frecuencias = defaultdict(int)
    for oracion in oraciones:
        oracion = oracion.split()
        for i in range(len(oracion) - 1):
            bigrama = (oracion[i], oracion[i + 1])
            frecuencias[bigrama] += 1
    return frecuencias

# Con las palabras anterior y posterior, y el diccionario de bigramas analiza cual es la palabra con mayor probabilidad
#   de estar entre esas dos palabras 
def predecir_palabra(palabra_anterior, palabra_posterior, frecuencias):
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

# Recibe una lista de frases incompletas (con un '_' donde iria la palabra que queremos predecir) y una lista de strings
#   que servira como informacion para predecir la palabra faltante
def completar_frases(frases_incompletas, oraciones):
    frecuencias = generar_bigramas(oraciones)
    frases_completas = []
    for oracion in frases_incompletas:
        oracion = oracion.split()
        for i, palabra in enumerate(oracion):
            if palabra == '_':
                anterior = oracion[i - 1] if i > 0 else ''
                posterior = oracion[i + 1] if i < len(oracion) - 1 else ''
                oracion[i] = predecir_palabra(anterior, posterior, frecuencias)
        frases_completas.append(' '.join(oracion))
    return frases_completas
    
def main():
    nombre_persona = sys.argv[1]

    oraciones = cargar_oraciones(nombre_persona)
    frases_incompletas = cargar_frases_incompletas(nombre_persona)
    frases_completas = completar_frases(frases_incompletas, oraciones)
    
    f = open(f"Salidas/{nombre_persona}.txt", "w")
    for frase in frases_completas:
        f.write(f"{frase}\n")
    
    

def test_generar_bigramas():
    oraciones_ejemplo = [
        "en mi cancion te has perdido",
        "no encontrarte trajo mala suerte",
        "y pregunto",
        "que paso que paso",
        "que paso que paso",
        "rezo rezo rezo rezo por vos"
    ]

    resultado = generar_bigramas(oraciones_ejemplo)

    assert resultado == {
        ("en", "mi"): 1,
        ("mi", "cancion"): 1,
        ("cancion", "te"): 1,
        ("te", "has"): 1,
        ("has", "perdido"): 1,
        ("no", "encontrarte"): 1,
        ("encontrarte", "trajo"): 1,
        ("trajo", "mala"): 1,
        ("mala", "suerte"): 1,
        ("y", "pregunto"): 1,
        ("que", "paso"): 4,
        ("paso", "que"): 2,
    }

def test_procesar_palabra():
    oraciones_ejemplo = [
        "en mi cancion te has perdido",
        "no encontrarte trajo mala suerte",
        "y pregunto",
        "que paso que paso",
        "que paso que paso",
        "rezo rezo rezo rezo por vos"
    ]
    assert predecir_palabra("trajo", "suerte", generar_bigramas(oraciones_ejemplo)) == "mala"
    assert predecir_palabra("que", "que", generar_bigramas(oraciones_ejemplo)) == "paso"
        
def test_completar_frases():
    oraciones_ejemplo = [
        "en mi cancion te has perdido",
        "no encontrarte trajo mala suerte",
        "y pregunto",
        "que paso que paso",
        "que paso que paso",
        "rezo rezo rezo rezo por vos"
    ]
    frases_incompletas = [
        "mi _",
        "que _ que paso",
        "paso _",
        "trajo _ suerte",
        "rezo _ vos"
    ]
    assert completar_frases(frases_incompletas, oraciones_ejemplo) == [
        "mi cancion",
        "que paso que paso",
        "paso que",
        "trajo mala suerte",
        "rezo por vos"
    ] 
    
    
if __name__ == "__main__":
    main()
