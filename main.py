import sys

def main():
    if len(sys.argv) < 2:
        print("Uso: python main.py <nombre>")
        return

    nombre_persona = sys.argv[1]
    print("El nombre proporcionado es:", nombre_persona)

if __name__ == "__main__":
    main()
