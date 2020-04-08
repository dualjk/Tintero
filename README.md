# Tintero

Progetto di **text editor cooperativo** online, parte del corso "programmazione di sistema" del politecnico di torino, laurea magistrale in ingegneria informatica, a.a. 2018/2019.

Il nome "Tintero" viene dalla parola spagnola che sta per "Calamaio", che ben riprende l'idea di fondo della nostra applicazione, ossia la realizzazione di un editor testo. Suonava bene. 


## Architettura

L'architettura è di tipo client-server: il server apre la connessione al database utenti, in più gestisce la scrittura e la cancellazione dei caratteri nel documento che il client, dopo essersi loggato, andrà a modificare.

## Idee generali

- partire innanzitutto verificando che una connessione client-server funzioni, di qualsiasi tipo essa sia (probabilmente inizieremo a implementare il login)


# Use cases
## Login
### Login riuscito
1. Utente ha già un account
2. Utente inserisce username e password correttamente
3. Utente entra

### Login fallito
1. Utente ha già un account
2. Utente inserisce username e password non correttamente
3. Utente non entra

### Login fallito ripetutamente
1. Utente ha già un account
2. Utente inserisce username e password non correttamente più di x volte
3. Utente non entra
4. Gli viene proposto di recuperare la pass oppure si blocca l'accesso

### creazione nuovo account
1. U inserisce nuovo possibile nick
2. Se è univoco viene accettato
3. U inserisce nuova possibile password
4. viene creato nuovo account
5. Torna ai casi 

## Scelta del documento da modificare
### Documenti presistenti
1. U sceglie documento da una lista
2. U apre documento 
3. Viene caricata **l'ultima versione disponibile del documento dal server**
4. U inizia a modificarlo

### Documento non presistente
1. U seleziona creazione nuovo documento
2. U assengna nome al nuuovo documento -> da discutere, possiamo anche assegnare un identificativo tipo chiave hash al nuovo documento ed evitare il nome, magari U lo può scegliere solo in fase di esportazione
3. U inizia a scrivere sul nuovo documento

## Modifica del documento (parte frizzantina)
### U aggiunge un carattere
1. U digita un carattere sulla sua tastiera
2. il carattere viene inserito localmente
3. il carattere viene inserito da remoto e la modifica viene propagata dal server in tutte le sessioni attive

### U rimuove un carattere
1. U rimuove un carattere
2. il carattere viene rimosso localmente
3. il carattere viene rimosso da remoto e la modifica viene propagata dal server in tutte le sessioni attive

### U aggiunge un arricchimento ad un carattere
1. U seleziona un carattere e lo arricchisce (grassetto corsivo sottolineato ecc)
2. il carattere viene arricchito localmente
3. il carattere viene arricchito da remoto e la modifica viene propagata dal server in tutte le sessioni attive

### U rimuove un arricchimento ad un carattere
1. U seleziona un carattere e lo impoverisce (?) (grassetto corsivo sottolineato ecc)
2. il carattere viene impoverito localmente
3. il carattere viene impoverito da remoto e la modifica viene propagata dal server in tutte le sessioni attive

### U performa una delle precedenti azioni su una stringa
* scomponiamo l'azione in tante piccole azioni fatte sui singoli caratteri (?) -> non lo so, potremmo avere problemi di performance ma temo non ci sia altra via


## Cose da capire
# Lato Text Editor (textEdit è la classe di QT per la gestione di un rich text editor)
+ come scrivere su un textEdit un array/lista
+ se implementare un array o una lista per conservare tutti i caratteri di un documento
+ come implementare l'algoritmo di conflict-free replicated data type crdt sulla nostra applicazione
+ se basta usare QChar nativo di Qt per implementare questa cosa o se vada incapsulato in una struttura più grosso 
	* quelli di conclave hanno usato una classe crdt che includeva un identificatore univoco e un carattere
	* se usiamo la stessa strategia, capire come possiamo farlo visualizzare su textEdit -> temo di no 


# Lato comunicazione tra processi
+ come implementare le quattro operazioni principali (cito da conclave)
    * **Local Insert**: User inserts character into their local text editor and sends the operation to all other users.
    * **Local Delete**: User deletes character from their local editor and sends the operation to all other users.
    * **Remote Insert**: User receives a insert operation from another user and inserts it to their local editor.
    * **Remote Delete**: User receives a delete operation from another user and deletes it from their local editor.


## Proposte di implementazione
1. **programmino stupido che scriva su textEdit una QString già inizializzata** -> fatto, funziona 
2. **programmino stupido che riesca ad inviare e a ricevere un QChar da un altro programmino stupido** -> fatto
3. **programmino stupido che riesca a generare un documento di testo arricchito (html?)**
4. **programmino molto stupido che legga un documento arricchito e lo faccia stampare da un textEdit**
5. **programmino stupido che restituisca TRUE se username e password sono presenti in un db (acquisizione username e psw, scrittura della query, scrittura/lettura del db)** -> fatto e sembra un sayan

### **TODO (AGGIORNATO AL 8/4/2020)**

+ AUTENTICAZIONE (REGISTRAZIONE, LOGIN)
 	* **path relativo database** -> non fatto
 	* **path relativo icona** -> fatto
 	* **upload avatar utente** -> fatto
 	* **cifratura della password** -> fatto
 	* **gestione utente non esistente** -> fatto
 	* **implementare metodo per la gestione della password non combaciante** -> fatto
 	* **inviare dal server al client messaggi per "avvenuta-azione"** -> fatto
 	* **terza pagina per mostrare le informazioni dell'utente loggato** -> fatto
 	* **gestire alcune problematiche legate all'immagine** -> aggirato
 	* **DOCUMENTAZIONE ASSOLUTAMENTE PERCHè NON SI CAPISCE UNA MINCHIA**
 	* **Area documenti scrollabile** -> bruttina ma fatto
 	* **Database con i documenti** -> fatto
 	* **Inviare al client la lista coi documenti dopo login**
 	* **Stampare la lista dei documenti**
 	* **Label sotto le icone dei documenti** -> fatto
 	* **Implementare controllo univocità nome documento da parte dello stesso utente (u1 non può creare due file chiamati entrambi "d1")**
