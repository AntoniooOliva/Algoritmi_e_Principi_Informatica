### Oliva Antonio
# Progetto API 2022 / 2023

Si consideri un’autostrada descritta come una sequenza di stazioni di servizio. Ogni stazione di servizio si trova ad una distanza
dall’inizio dell’autostrada espressa in chilometri da un numero intero positivo o nullo. Non esistono due stazioni di servizio aventi
la stessa distanza: ogni stazione di servizio è quindi univocamente identificata dalla sua distanza dall’inizio dell’autostrada.

Ogni stazione di servizio è dotata di un parco veicoli elettrici a noleggio. Ogni veicolo è contraddistinto dall’autonomia data da una carica delle
batterie, espressa in chilometri, da un numero intero positivo. Il parco veicoli di una singola stazione comprende al più 512 veicoli. Presa a
noleggio un’auto da una stazione s, è possibile raggiungere tutte quelle la cui distanza da s è minore o uguale all’autonomia dell’automobile.

Un viaggio è identificato da una sequenza di stazioni di servizio in cui il conducente fa tappa. Ha quindi inizio in una stazione di
servizio e termina in un’altra, passando per zero o più stazioni intermedie. Si assuma che il conducente non possa tornare indietro
durante il viaggio, e noleggi un’auto nuova ogniqualvolta faccia tappa in una stazione di servizio. Quindi, date due tappe consecutive s e t,
t deve essere sempre più lontana dalla partenza rispetto a s, e t deve essere raggiungibile usando uno dei veicoli disponibili in s.

L’obiettivo del progetto è il seguente: data una coppia di stazioni, pianificare il percorso con il minor numero
di tappe tra di esse. Nel caso siano presenti più percorsi con lo stesso numero minimo di tappe (cioè a pari merito),
deve essere scelto il percorso che predilige le tappe con distanza più breve dall’inizio dell’autostrada.

#### Voto finale: 30L
