"""
Validacion lexica basado en un AFD
Regla formal: [A-Z][a-z]+[0-9]
"""
from typing import Dict, Tuple

def clasificarSimbolo(caracter: str) -> str:
    """Clasifica un carácter de entrada en uno de los 4 símbolos del alfabeto Σ."""
    if 'A' <= caracter <= 'Z':
        return 'M'  # Mayuscula
    elif 'a' <= caracter <= 'z':
        return 'm'  # Minuscula
    elif '0' <= caracter <= '9':
        return 'd'  # Digito
    else:
        return 'o'  # invalido

# Función de transicion 
TRANSICIONES: Dict[Tuple[str, str], str] = {
    # Desde q0 
    ('q0', 'M'): 'q1',
    ('q0', 'm'): 'q_err',
    ('q0', 'd'): 'q_err',
    ('q0', 'o'): 'q_err',

    # Desde q1 mayuscula
    ('q1', 'M'): 'q_err',
    ('q1', 'm'): 'q2',
    ('q1', 'd'): 'q_err',
    ('q1', 'o'): 'q_err',

    # Desde q2 al menos una minuscula
    ('q2', 'M'): 'q_err',
    ('q2', 'm'): 'q2',
    ('q2', 'd'): 'q3',
    ('q2', 'o'): 'q_err',

    # Desde q3 estado de aceptación dígito final leído
    ('q3', 'M'): 'q_err',
    ('q3', 'm'): 'q_err',
    ('q3', 'd'): 'q_err',
    ('q3', 'o'): 'q_err',

    # Desde q_err estado de rechazo o trampa
    ('q_err', 'M'): 'q_err',
    ('q_err', 'm'): 'q_err',
    ('q_err', 'd'): 'q_err',
    ('q_err', 'o'): 'q_err',
}

MENSAJES_ERROR_TRANSICION = {
    'q0': "Se esperaba una letra mayuscula al inicio.",
    'q1': "Se esperaba al menos una letra minúscula (a-z) después de la mayuscula inicial.",
    'q2': "Se esperaba otra letra minuscula o un unico dígito final (0-9).",
    'q3': "La etiqueta ya había terminado con un digito; no se permiten más caracteres",
}

MENSAJES_ERROR_INCOMPLETO = {
    'q0': "Cadena vacia: falta la mayuscula inicial, las minusculas y el digito final.",
    'q1': "Cadena incompleta: falta al menos una letra minuscula y el digito final",
    'q2': "Cadena incompleta: falta el dígito final obligatorio",
}


def validarEtiqueta(cadena: str) -> dict:
    """
    Procesa una cadena simbolo a síimbolo usando el AFD.
    """
    estadoActual = 'q0'
    estadoAceptacion = 'q3'

    for indice, caracter in enumerate(cadena):
        simbolo = clasificarSimbolo(caracter)
        siguienteEstado = TRANSICIONES[(estadoActual, simbolo)]

        if siguienteEstado == 'q_err':
            detalle = MENSAJES_ERROR_TRANSICION.get(estadoActual, "Caracter invalido.")
            return {
                "etiqueta": cadena,
                "valida": False,
                "estado_final": "q_err",
                "motivo": (
                    f"Rechazada en estado '{estadoActual}' al leer el carácter '{caracter}' "
                    f"(posición {indice}). {detalle}"
                ),
            }

        estadoActual = siguienteEstado

    # Fin de la cadena: verificar si quedó en estado de aceptación
    if estadoActual == estadoAceptacion:
        return {
            "etiqueta": cadena,
            "valida": True,
            "estado_final": estadoActual,
            "motivo": "Aceptada: cumple con la regla formal [A-Z][a-z]+[0-9].",
        }
    else:
        detalle = MENSAJES_ERROR_INCOMPLETO[estadoActual]
        return {
            "etiqueta": cadena,
            "valida": False,
            "estado_final": estadoActual,
            "motivo": f"Rechazada al finalizar la lectura en estado '{estadoActual}'. {detalle}",
        }


if __name__ == "__main__":
    # Casos de prueba base del enunciado + casos límite
    print("=== VALIDADOR DE ETIQUETAS (AFD) ===")
    print("Regla formal: [A-Z][a-z]+[0-9] (Ej: Get1, Run2)")
    print("Ingresa 5 etiquetas para evaluar:\n")

    casos_usuario = []
    for i in range(1, 6):
        palabra = input(f"Etiqueta {i}/5: ")
        casos_usuario.append(palabra)

    print(f"{'ETIQUETA':<10} | {'ESTADO':<8} | {'RESULTADO':<10} | DIAGNOSTICO")
    print("-" * 95)
    for prueba in casos_usuario:
        res = validarEtiqueta(prueba)
        estado_txt = "VÁLIDA" if res["valida"] else "INVÁLIDA"
        etiqueta_mostrar = repr(prueba) if prueba == "" else prueba
        print(f"{etiqueta_mostrar:<10} | {res['estado_final']:<8} | {estado_txt:<10} | {res['motivo']}")