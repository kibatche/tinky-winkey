# tinky-winkey
Un service Windows qui permet d'enregistrer les frappes du clavier.

## Fonctionnement

On a deux programmes : tinky et winkey.

Le premier met en place un service windows avec les plus hauts priuvlèges possibles, [en créant un token](https://github.com/kibatche/tinky-winkey/blob/5839e9e7716ecfa666df9025ea087043909a92cd/svc/srcs/impersonate_token.c#L188) ayant tous les droits possibles. Pour se faire, on duplique le token du processus `winlogon.exe`. Une fois dupliqué, on active tous les droits de ce token, et on lance le keylogger avec ces droits (ceux NT_AUTHORITY_SYSTEM).

Le cycle de vie du service est géré de bout en bout : création, démarrage, mise à jour, arrêt, suppression. On peut également mettre en place une méthode pour cacher le processus du keylogger du gestionnaire de processus windows (on reste dans le _userland_, donc impossible de se cacher d'un programme tel que _procmon_). Pour terminer, on peut établir un shell à distance.

Le programme de keylogging quant à lui permet de :

- loguer les touches (combinaisons de touches incluses)
- loguer le nom de la fenêtre
- le nom de l'utilisateur de la session
- horodater ces informations
- de prendre une capture d'écran à chaque changement de fenêtre.
