const fs = require('node:fs');
const inicioPrograma = performance.now()

const  a = 1, b = 2, c = 3, d = 4, e = 5, f = 6, g = 7, h = 8

/*const movimiento1 = [
    { columna: c, fila: 2 }, 
    { columna: d, fila: 4 },
    { columna: b, fila: 3 },
    { columna: c, fila: 5 },
]*/

const movimiento1 = [
  { columna: b, fila: 4 }, { columna: a, fila: 6 }, { columna: c, fila: 5 }, { columna: a, fila: 4 },
  { columna: c, fila: 3 }, { columna: a, fila: 2 }, { columna: c, fila: 1 }, { columna: e, fila: 2 },
  { columna: g, fila: 1 }, { columna: h, fila: 3 }, { columna: g, fila: 5 }, { columna: e, fila: 6 },
  { columna: c, fila: 7 }, { columna: a, fila: 8 }, { columna: c, fila: 7 }, { columna: b, fila: 5 },
  { columna: d, fila: 6 }, { columna: b, fila: 7 }, { columna: a, fila: 5 }, { columna: c, fila: 4 },
  { columna: a, fila: 3 }, { columna: c, fila: 2 }, { columna: a, fila: 1 }, { columna: c, fila: 2 },
  { columna: b, fila: 4 }, { columna: a, fila: 6 }, { columna: b, fila: 8 }, { columna: d, fila: 7 },
  { columna: b, fila: 6 }, { columna: a, fila: 8 }, { columna: c, fila: 7 }, { columna: a, fila: 6 },
  { columna: c, fila: 5 }, { columna: a, fila: 4 }, { columna: c, fila: 3 }, { columna: a, fila: 2 },
  { columna: c, fila: 1 }, { columna: e, fila: 2 }, { columna: g, fila: 3 }, { columna: h, fila: 5 },
  { columna: g, fila: 7 }, { columna: e, fila: 8 }, { columna: c, fila: 7 }, { columna: a, fila: 8 },
  { columna: c, fila: 7 }, { columna: b, fila: 5 }, { columna: d, fila: 4 }, { columna: b, fila: 3 },
  { columna: a, fila: 1 }, { columna: c, fila: 2 }, { columna: a, fila: 3 }, { columna: c, fila: 4 },
  { columna: a, fila: 5 }, { columna: c, fila: 6 }, { columna: a, fila: 7 }, { columna: c, fila: 8 },
  { columna: a, fila: 7 }, { columna: c, fila: 6 }, { columna: a, fila: 5 }, { columna: c, fila: 4 },
  { columna: a, fila: 3 }, { columna: c, fila: 2 }, { columna: a, fila: 1 }, { columna: c, fila: 2 },
  { columna: b, fila: 4 }, { columna: d, fila: 5 }, { columna: b, fila: 6 }, { columna: a, fila: 8 },
  { columna: c, fila: 7 }, { columna: a, fila: 6 }, { columna: c, fila: 5 }, { columna: a, fila: 4 },
  { columna: c, fila: 3 }, { columna: a, fila: 2 }, { columna: c, fila: 1 }, { columna: e, fila: 2 },
  { columna: g, fila: 1 }, { columna: h, fila: 3 }, { columna: g, fila: 5 }, { columna: e, fila: 6 },
  { columna: c, fila: 5 }, { columna: a, fila: 4 }, { columna: c, fila: 3 }, { columna: a, fila: 2 },
  { columna: c, fila: 1 }, { columna: e, fila: 2 }, { columna: g, fila: 3 }, { columna: h, fila: 5 },
  { columna: g, fila: 7 }, { columna: e, fila: 8 }, { columna: c, fila: 7 }, { columna: a, fila: 8 },
  { columna: c, fila: 7 }, { columna: b, fila: 5 }, { columna: d, fila: 6 }, { columna: b, fila: 7 },
  { columna: a, fila: 5 }, { columna: c, fila: 4 }, { columna: a, fila: 3 }, { columna: c, fila: 2 },
  { columna: a, fila: 1 }, { columna: c, fila: 2 }, { columna: b, fila: 4 }, { columna: a, fila: 6 },
  { columna: b, fila: 8 }, { columna: d, fila: 7 }, { columna: b, fila: 6 }, { columna: a, fila: 8 },
  { columna: c, fila: 7 }, { columna: a, fila: 6 }, { columna: c, fila: 5 }, { columna: a, fila: 4 },
  { columna: c, fila: 3 }, { columna: a, fila: 2 }, { columna: c, fila: 1 }, { columna: e, fila: 2 },
  { columna: g, fila: 3 }, { columna: h, fila: 5 }, { columna: g, fila: 7 }, { columna: e, fila: 8 },
  { columna: c, fila: 7 }, { columna: a, fila: 8 }, { columna: c, fila: 7 }, { columna: b, fila: 5 },
  { columna: d, fila: 4 }, { columna: b, fila: 3 }, { columna: a, fila: 1 }, { columna: c, fila: 2 },
  { columna: a, fila: 3 }, { columna: c, fila: 4 }, { columna: a, fila: 5 }, { columna: c, fila: 6 },
  { columna: a, fila: 7 }, { columna: c, fila: 8 }, { columna: a, fila: 7 }, { columna: c, fila: 6 },
  { columna: a, fila: 5 }, { columna: c, fila: 4 }, { columna: a, fila: 3 }, { columna: c, fila: 2 },
  { columna: a, fila: 1 }, { columna: c, fila: 2 }, { columna: b, fila: 4 }, { columna: d, fila: 5 },
  { columna: b, fila: 6 }, { columna: a, fila: 8 }, { columna: c, fila: 7 }, { columna: a, fila: 6 },
  { columna: c, fila: 5 }, { columna: a, fila: 4 }, { columna: c, fila: 3 }, { columna: a, fila: 2 },
  { columna: c, fila: 1 }, { columna: e, fila: 2 }, { columna: g, fila: 1 }, { columna: h, fila: 3 },
  { columna: g, fila: 5 }, { columna: e, fila: 6 }, { columna: c, fila: 5 }, { columna: a, fila: 4 },
  { columna: c, fila: 3 }, { columna: a, fila: 2 }, { columna: c, fila: 1 }, { columna: e, fila: 2 },
  { columna: g, fila: 3 }, { columna: h, fila: 5 }, { columna: g, fila: 7 }, { columna: e, fila: 8 },
  { columna: c, fila: 7 }, { columna: a, fila: 8 }, { columna: c, fila: 7 }, { columna: b, fila: 5 },
  { columna: d, fila: 6 }, { columna: b, fila: 7 }, { columna: a, fila: 5 }, { columna: c, fila: 4 },
  { columna: a, fila: 3 }, { columna: c, fila: 2 }, { columna: a, fila: 1 }, { columna: c, fila: 2 },
  { columna: b, fila: 4 }, { columna: a, fila: 6 }, { columna: b, fila: 8 }, { columna: d, fila: 7 },
  { columna: b, fila: 6 }, { columna: a, fila: 8 }, { columna: c, fila: 7 }, { columna: a, fila: 6 },
  { columna: c, fila: 5 }, { columna: a, fila: 4 }, { columna: c, fila: 3 }, { columna: a, fila: 2 },
  { columna: c, fila: 1 }, { columna: e, fila: 2 }, { columna: g, fila: 3 }, { columna: h, fila: 5 },
  { columna: g, fila: 7 }, { columna: e, fila: 8 }, { columna: c, fila: 7 }, { columna: a, fila: 8 },
  { columna: c, fila: 7 }, { columna: b, fila: 5 }, { columna: d, fila: 4 }, { columna: b, fila: 3 },
  { columna: a, fila: 1 }, { columna: c, fila: 2 }, { columna: a, fila: 3 }, { columna: c, fila: 4 },
  { columna: a, fila: 5 }, { columna: c, fila: 6 }, { columna: a, fila: 7 }, { columna: c, fila: 8 },
  { columna: a, fila: 7 }, { columna: c, fila: 6 }, { columna: a, fila: 5 }, { columna: c, fila: 4 },
  { columna: a, fila: 3 }, { columna: c, fila: 2 }, { columna: a, fila: 1 }, { columna: c, fila: 2 },
  { columna: b, fila: 4 }, { columna: d, fila: 5 }, { columna: b, fila: 6 }, { columna: a, fila: 8 },
  { columna: c, fila: 7 }, { columna: a, fila: 6 }, { columna: c, fila: 5 }, { columna: a, fila: 4 },
  { columna: c, fila: 3 }, { columna: a, fila: 2 }, { columna: c, fila: 1 }, { columna: e, fila: 2 },
  { columna: g, fila: 3 }, { columna: h, fila: 5 }, { columna: g, fila: 7 }, { columna: e, fila: 8 },
  { columna: c, fila: 7 }, { columna: a, fila: 8 }, { columna: c, fila: 7 }, { columna: b, fila: 5 },
  { columna: d, fila: 4 }, { columna: b, fila: 3 }, { columna: a, fila: 1 }, { columna: c, fila: 2 },
  { columna: a, fila: 3 }, { columna: c, fila: 4 }, { columna: a, fila: 5 }, { columna: c, fila: 6 },
  { columna: a, fila: 7 }, { columna: c, fila: 8 }, { columna: a, fila: 7 }, { columna: c, fila: 6 },
  { columna: a, fila: 5 }, { columna: c, fila: 4 }, { columna: a, fila: 3 }, { columna: c, fila: 2 },
  { columna: a, fila: 1 }, { columna: c, fila: 2 }, { columna: b, fila: 4 }, { columna: d, fila: 5 },
  { columna: b, fila: 6 }, { columna: a, fila: 8 }, { columna: c, fila: 7 }, { columna: a, fila: 6 },
  { columna: c, fila: 5 }, { columna: a, fila: 4 }, { columna: c, fila: 3 }, { columna: a, fila: 2 },
  { columna: c, fila: 1 }, { columna: e, fila: 2 }, { columna: g, fila: 1 }, { columna: h, fila: 3 },
  { columna: g, fila: 5 }, { columna: e, fila: 6 }, { columna: c, fila: 5 }, { columna: a, fila: 4 },
  { columna: c, fila: 3 }, { columna: a, fila: 2 }, { columna: c, fila: 1 }, { columna: e, fila: 2 },
  { columna: g, fila: 3 }, { columna: h, fila: 5 }, { columna: g, fila: 7 }, { columna: e, fila: 8 },
  { columna: c, fila: 7 }, { columna: a, fila: 8 }, { columna: c, fila: 7 }, { columna: b, fila: 5 },
  { columna: d, fila: 6 }, { columna: b, fila: 7 }, { columna: a, fila: 5 }, { columna: c, fila: 4 },
  { columna: a, fila: 3 }, { columna: c, fila: 2 }, { columna: a, fila: 1 }, { columna: c, fila: 2 },
  { columna: b, fila: 4 }, { columna: a, fila: 6 }, { columna: b, fila: 8 }, { columna: d, fila: 7 },
  { columna: b, fila: 6 }, { columna: a, fila: 8 }, { columna: c, fila: 7 }, { columna: a, fila: 6 },
  { columna: c, fila: 5 }, { columna: a, fila: 4 }, { columna: c, fila: 3 }, { columna: a, fila: 2 },
  { columna: c, fila: 1 }, { columna: e, fila: 2 }, { columna: g, fila: 3 }, { columna: h, fila: 5 },
  { columna: g, fila: 7 }, { columna: e, fila: 8 }, { columna: c, fila: 7 }, { columna: a, fila: 8 },
  { columna: c, fila: 7 }, { columna: b, fila: 5 }, { columna: d, fila: 4 }, { columna: b, fila: 3 },
  { columna: a, fila: 1 }, { columna: c, fila: 2 }, { columna: a, fila: 3 }, { columna: c, fila: 4 },
  { columna: a, fila: 5 }, { columna: c, fila: 6 }, { columna: a, fila: 7 }, { columna: c, fila: 8 },
  { columna: a, fila: 7 }, { columna: c, fila: 6 }, { columna: a, fila: 5 }, { columna: c, fila: 4 },
  { columna: a, fila: 3 }, { columna: c, fila: 2 }, { columna: a, fila: 1 }, { columna: c, fila: 2 },
  { columna: b, fila: 4 }, { columna: d, fila: 5 }, { columna: b, fila: 6 }, { columna: a, fila: 8 },
  { columna: c, fila: 7 }, { columna: a, fila: 6 }, { columna: c, fila: 5 }, { columna: a, fila: 4 },
  { columna: c, fila: 3 }, { columna: a, fila: 2 }, { columna: c, fila: 1 }, { columna: e, fila: 2 },
  { columna: g, fila: 1 }, { columna: h, fila: 3 }, { columna: g, fila: 5 }, { columna: e, fila: 6 },
  { columna: c, fila: 5 }, { columna: a, fila: 4 }, { columna: c, fila: 3 }, { columna: a, fila: 2 },
  { columna: c, fila: 1 }, { columna: e, fila: 2 }, { columna: g, fila: 3 }, { columna: h, fila: 5 },
  { columna: g, fila: 7 }, { columna: e, fila: 8 }, { columna: c, fila: 7 }, { columna: a, fila: 8 },
  { columna: c, fila: 7 }, { columna: b, fila: 5 }, { columna: d, fila: 6 }, { columna: b, fila: 7 },
  { columna: a, fila: 5 }, { columna: c, fila: 4 }, { columna: a, fila: 3 }, { columna: c, fila: 2 },
  { columna: a, fila: 1 }, { columna: c, fila: 2 }, { columna: b, fila: 4 }, { columna: a, fila: 6 },
  { columna: b, fila: 8 }, { columna: d, fila: 7 }, { columna: b, fila: 6 }, { columna: a, fila: 8 },
  { columna: c, fila: 7 }, { columna: a, fila: 6 }, { columna: c, fila: 5 }, { columna: a, fila: 4 },
  { columna: c, fila: 3 }, { columna: a, fila: 2 }, { columna: c, fila: 1 }, { columna: e, fila: 2 },
  { columna: g, fila: 3 }, { columna: h, fila: 5 }, { columna: g, fila: 7 }, { columna: e, fila: 8 },
  { columna: c, fila: 7 }, { columna: a, fila: 8 }, { columna: c, fila: 7 }, { columna: b, fila: 5 },
  { columna: d, fila: 4 }, { columna: b, fila: 3 }, { columna: a, fila: 1 }, { columna: c, fila: 2 },
  { columna: a, fila: 3 }, { columna: c, fila: 4 }, { columna: a, fila: 5 }, { columna: c, fila: 6 },
  { columna: a, fila: 7 }, { columna: c, fila: 8 }, { columna: a, fila: 7 }, { columna: c, fila: 6 },
  { columna: a, fila: 5 }, { columna: c, fila: 4 }, { columna: a, fila: 3 }, { columna: c, fila: 2 },
  { columna: a, fila: 1 }, { columna: c, fila: 2 }, { columna: b, fila: 4 }, { columna: d, fila: 5 },
  { columna: b, fila: 6 }, { columna: a, fila: 8 }, { columna: c, fila: 7 }, { columna: a, fila: 6 },
  { columna: c, fila: 5 }, { columna: a, fila: 4 }, { columna: c, fila: 3 }, { columna: a, fila: 2 },
  { columna: c, fila: 1 }, { columna: e, fila: 2 }, { columna: g, fila: 1 }, { columna: h, fila: 3 },
  { columna: g, fila: 5 }, { columna: e, fila: 6 }, { columna: c, fila: 5 }, { columna: a, fila: 4 },
  { columna: c, fila: 3 }, { columna: a, fila: 2 }, { columna: c, fila: 1 }, { columna: e, fila: 2 },
  { columna: g, fila: 3 }, { columna: h, fila: 5 }, { columna: g, fila: 7 }, { columna: e, fila: 8 },
  { columna: c, fila: 7 }, { columna: a, fila: 8 }, { columna: c, fila: 7 }, { columna: b, fila: 5 },
  { columna: d, fila: 6 }, { columna: b, fila: 7 }, { columna: a, fila: 5 }, { columna: c, fila: 4 },
  { columna: a, fila: 3 }, { columna: c, fila: 2 }, { columna: a, fila: 1 }, { columna: c, fila: 2 },
  { columna: b, fila: 4 }, { columna: a, fila: 6 }, { columna: b, fila: 8 }, { columna: d, fila: 7 },
  { columna: b, fila: 6 }, { columna: a, fila: 8 }, { columna: c, fila: 7 }, { columna: a, fila: 6 },
  { columna: c, fila: 5 }, { columna: a, fila: 4 }, { columna: c, fila: 3 }, { columna: a, fila: 2 },
  { columna: c, fila: 1 }, { columna: e, fila: 2 }, { columna: g, fila: 3 }, { columna: h, fila: 5 },
  { columna: g, fila: 7 }, { columna: e, fila: 8 }, { columna: c, fila: 7 }, { columna: a, fila: 8 },
  { columna: c, fila: 7 }, { columna: b, fila: 5 }, { columna: d, fila: 4 }, { columna: b, fila: 3 },
  { columna: a, fila: 1 }, { columna: c, fila: 2 }, { columna: a, fila: 3 }, { columna: c, fila: 4 },
  { columna: a, fila: 5 }, { columna: c, fila: 6 }, { columna: a, fila: 7 }, { columna: c, fila: 8 },
  { columna: a, fila: 7 }, { columna: c, fila: 6 }, { columna: a, fila: 5 }, { columna: c, fila: 4 },
  { columna: a, fila: 3 }, { columna: c, fila: 2 }, { columna: a, fila: 1 }, { columna: c, fila: 2 },
  { columna: b, fila: 4 }, { columna: d, fila: 5 }, { columna: b, fila: 6 }, { columna: a, fila: 8 },
  { columna: c, fila: 7 }, { columna: a, fila: 6 }, { columna: c, fila: 5 }, { columna: a, fila: 4 }
];



const movimientoValido = (movimiento) => {
    let movimientoPrevio = null
    for (let index = 0; index < movimiento.length; index++) { //itera por todos los movimientos
        if (movimientoPrevio) {
            if ((movimientoPrevio.columna == movimiento[index].columna + 2) || (movimientoPrevio.columna == movimiento[index].columna - 2) && ((movimientoPrevio.columna > 0) && (movimientoPrevio.columna < 9))) {
                if ((movimientoPrevio.fila == movimiento[index].fila + 1 )|| (movimientoPrevio.fila == movimiento[index].fila - 1) && (movimientoPrevio.fila > 0) && (movimientoPrevio.fila < 9)) {
                    console.log("Movimiento valido", movimiento[index]) //revisa movimiento en L 
                } else {
                    console.log("Movimiento invalido", movimiento[index])
                    return 
                }
            } else if ((movimientoPrevio.columna == movimiento[index].columna + 1) || (movimientoPrevio.columna == movimiento[index].columna - 1) && (movimientoPrevio.columna > 0) && (movimientoPrevio.columna < 9)) {
                if ((movimientoPrevio.fila == movimiento[index].fila + 2 )|| (movimientoPrevio.fila == movimiento[index].fila - 2 ) && (movimientoPrevio.fila > 0) && (movimientoPrevio.fila < 9)) {
                    console.log("Movimiento valido", movimiento[index]) //revisa movimiento en L
                } else {
                    console.log("Movimiento invalido", movimiento[index])
                    return 
                }
            } else {
                    console.log("Movimiento invalido", movimiento[index])
                    return 
                }
        } 
        movimientoPrevio = movimiento[index] //guarda el movimiento actual como movimiento previo para compararlo en la siguiente iteracion
    }
}



movimientoValido(movimiento1)

const finPrograma = performance.now()

const tiempoTotal = finPrograma - inicioPrograma
console.log(`El programa duró ${String(tiempoTotal)}ms`,)