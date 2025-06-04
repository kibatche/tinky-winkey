#pragma once

#define SVC_NAME "tinky"

// VK_LBUTTON 	0x01 	Bouton gauche de la souris
// VK_RBUTTON 	0x02 	Bouton droit de la souris
// VK_CANCEL 	0x03 	Traitement d’arrêt de contrôle
// VK_MBUTTON 	0x04 	Bouton de la souris centrale
// VK_XBUTTON1 	0x05 	Bouton de la souris X1
// VK_XBUTTON2 	0x06 	Bouton de la souris X2
// 	0x07 	Réservé
// VK_BACK 	0x08 	Touche d’arrière-arrière
// VK_TAB 	0x09 	Touche Tab
// 	0x0A-0B 	Réservé
// VK_CLEAR 	0x0C 	Effacer la clé
// VK_RETURN 	0x0D 	Entrer la clé
// 	0x0E-0F 	Non attribué
// VK_SHIFT 	0x10 	Touche majuscule
// VK_CONTROL 	0x11 	Touche Ctrl
// VK_MENU 	0x12 	Touche Alt
// VK_PAUSE 	0x13 	Suspendre la clé
// VK_CAPITAL 	0x14 	Touche de verrouillage des majuscules
// VK_KANA 	0x15 	Mode Kana IME
// VK_HANGUL 	0x15 	Mode Hangul IME
// VK_IME_ON 	0x16 	IME activé
// VK_JUNJA 	0x17 	IME Junja mode
// VK_FINAL 	0x18 	Mode final IME
// VK_HANJA 	0x19 	Mode IME Hanja
// VK_KANJI 	0x19 	Mode IME Kanji
// VK_IME_OFF 	0x1A 	IME Désactivé
// VK_ESCAPE 	0x1B 	Touche Échap
// VK_CONVERT 	0x1C 	Conversion IME
// VK_NONCONVERT 	0x1D 	IME nonconverti
// VK_ACCEPT 	0x1E 	IME accepter
// VK_MODECHANGE 	0x1F 	Demande de modification du mode IME
// VK_SPACE 	0x20 	Touche de barre d’espace
// VK_PRIOR 	0x21 	Touche de page vers le haut
// VK_NEXT 	0x22 	Touche vers le bas de page
// VK_END 	0x23 	Clé de fin
// VK_HOME 	0x24 	Clé d’accueil
// VK_LEFT 	0x25 	Flèche gauche
// VK_UP 	0x26 	Flèche vers le haut
// VK_RIGHT 	0x27 	Flèche droite
// VK_DOWN 	0x28 	Flèche vers le bas
// VK_SELECT 	0x29 	Sélectionner une clé
// VK_PRINT 	0x2A 	Touche d’impression
// VK_EXECUTE 	0x2B 	Exécuter la clé
// VK_SNAPSHOT 	0x2C 	Touche d’écran d’impression
// VK_INSERT 	0x2D 	Insérer une clé
// VK_DELETE 	0x2E 	Supprimer la clé
// VK_HELP 	0x2F 	Clé d’aide
// `0` 	0x30 	0 touche
// `1` 	0x31 	1 clé
// `2` 	0x32 	2 touches
// `3` 	0x33 	3 touches
// `4` 	0x34 	4 touches
// `5` 	0x35 	5 touches
// `6` 	0x36 	6 touches
// `7` 	0x37 	7 touches
// `8` 	0x38 	8 touches
// `9` 	0x39 	9 touches
// 	0x3A-40 	Indéfini
// `A` 	0x41 	Une clé
// `B` 	0x42 	Touche B
// `C` 	0x43 	Touche C
// `D` 	0x44 	Touche D
// `E` 	0x45 	Touche E
// `F` 	0x46 	Touche F
// `G` 	0x47 	Touche G
// `H` 	0x48 	Touche H
// `I` 	0x49 	J’ai la clé
// `J` 	0x4A 	Touche J
// `K` 	0x4B 	Clé K
// `L` 	0x4C 	Touche L
// `M` 	0x4D 	Touche M
// `N` 	0x4E 	Touche N
// `O` 	0x4F 	Touche O
// `P` 	0x50 	Touche P
// `Q` 	0x51 	Touche Q
// `R` 	0x52 	Touche R
// `S` 	0x53 	Touche S
// `T` 	0x54 	Touche T
// `U` 	0x55 	Touche U
// `V` 	0x56 	Clé V
// `W` 	0x57 	Touche W
// `X` 	0x58 	Touche X
// `Y` 	0x59 	Touche Y
// `Z` 	0x5A 	Touche Z

// VK_LWIN 	0x5B 	Touche de logo Windows gauche
// VK_RWIN 	0x5C 	Touche de logo Windows droite
// VK_APPS 	0x5D 	Clé d’application
// 	0x5E 	Réservé
// VK_SLEEP 	0x5F 	Clé de veille de l’ordinateur
// VK_NUMPAD0 	0x60 	Touche numérique 0 du pavé numérique
// VK_NUMPAD1 	0x61 	Touche numérique 1 du pavé numérique
// VK_NUMPAD2 	0x62 	Touche numérique 2 du pavé numérique
// VK_NUMPAD3 	0x63 	Touche numérique 3 du pavé numérique
// VK_NUMPAD4 	0x64 	Pavé numérique 4 touches
// VK_NUMPAD5 	0x65 	Touche numérique 5 du pavé numérique
// VK_NUMPAD6 	0x66 	Pavé numérique 6 touches
// VK_NUMPAD7 	0x67 	Touche numérique 7 du pavé numérique
// VK_NUMPAD8 	0x68 	Touche numérique 8 du pavé numérique
// VK_NUMPAD9 	0x69 	Touche numérique 9 du pavé numérique
// VK_MULTIPLY 	0x6A 	Multiplier la clé
// VK_ADD 	0x6B 	Ajouter une clé
// VK_SEPARATOR 	0x6C 	Clé de séparateur
// VK_SUBTRACT 	0x6D 	Soustraire la clé
// VK_DECIMAL 	0x6E 	Clé décimale
// VK_DIVIDE 	0x6F 	Diviser la clé
// VK_F1 	0x70 	Touche F1
// VK_F2 	0x71 	Touche F2
// VK_F3 	0x72 	Touche F3
// VK_F4 	0x73 	Touche F4
// VK_F5 	0x74 	Touche F5
// VK_F6 	0x75 	Touche F6
// VK_F7 	0x76 	Touche F7
// VK_F8 	0x77 	Touche F8
// VK_F9 	0x78 	Touche F9
// VK_F10 	0x79 	Touche F10
// VK_F11 	0x7A 	Touche F11
// VK_F12 	0x7B 	Touche F12
// VK_F13 	0x7C 	Touche F13
// VK_F14 	0x7D 	Touche F14
// VK_F15 	0x7E 	Touche F15
// VK_F16 	0x7F 	Touche F16
// VK_F17 	0x80 	Touche F17
// VK_F18 	0x81 	Touche F18
// VK_F19 	0x82 	Touche F19
// VK_F20 	0x83 	Touche F20
// VK_F21 	0x84 	Touche F21
// VK_F22 	0x85 	Touche F22
// VK_F23 	0x86 	Touche F23
// VK_F24 	0x87 	Touche F24
// 	0x88-8F 	Réservé
// VK_NUMLOCK 	0x90 	Touche de verrouillage num
// VK_SCROLL 	0x91 	Touche de verrouillage de défilement
// 	0x92-96 	Spécifique à l’OEM
// 	0x97-9F 	Non attribué
// VK_LSHIFT 	0xA0 	Touche Maj gauche
// VK_RSHIFT 	0xA1 	Touche Maj droite
// VK_LCONTROL 	0xA2 	Touche Ctrl gauche
// VK_RCONTROL 	0xA3 	Touche Ctrl droite
// VK_LMENU 	0xA4 	Touche Alt gauche
// VK_RMENU 	0xA5 	Touche Alt droite
// VK_BROWSER_BACK 	0xA6 	Touche Précédent du navigateur
// VK_BROWSER_FORWARD 	0xA7 	Touche Transférer du navigateur
// VK_BROWSER_REFRESH 	0xA8 	Touche d’actualisation du navigateur
// VK_BROWSER_STOP 	0xA9 	Touche d’arrêt du navigateur
// VK_BROWSER_SEARCH 	0xAA 	Touche de recherche du navigateur
// VK_BROWSER_FAVORITES 	0xAB 	Touche Favoris du navigateur
// VK_BROWSER_HOME 	0xAC 	Touche d’accueil et d’accueil du navigateur
// VK_VOLUME_MUTE 	0xAD 	Touche de désactivation du volume
// VK_VOLUME_DOWN 	0xAE 	Clé vers le bas du volume
// VK_VOLUME_UP 	0xAF 	Clé de volume vers le haut
// VK_MEDIA_NEXT_TRACK 	0xB0 	Touche De suivi suivante
// VK_MEDIA_PREV_TRACK 	0xB1 	Clé de suivi précédente
// VK_MEDIA_STOP 	0xB2 	Arrêter la clé multimédia
// VK_MEDIA_PLAY_PAUSE 	0xB3 	Lire/suspendre la clé multimédia
// VK_LAUNCH_MAIL 	0xB4 	Démarrer la clé de messagerie
// VK_LAUNCH_MEDIA_SELECT 	0xB5 	Sélectionner la clé multimédia
// VK_LAUNCH_APP1 	0xB6 	Démarrer la clé Application 1
// VK_LAUNCH_APP2 	0xB7 	Démarrer la clé Application 2
// 	0xB8-B9 	Réservé
// VK_OEM_1 	0xBA 	Utilisé pour les caractères divers ; il peut varier par clavier. Pour le clavier standard américain, la touche ;:
// VK_OEM_PLUS 	0xBB 	Pour n’importe quel pays/région, la clé +
// VK_OEM_COMMA 	0xBC 	Pour n’importe quel pays/région, la clé ,
// VK_OEM_MINUS 	0xBD 	Pour n’importe quel pays/région, la clé -
// VK_OEM_PERIOD 	0xBE 	Pour n’importe quel pays/région, la clé .
// VK_OEM_2 	0xBF 	Utilisé pour les caractères divers ; il peut varier par clavier. Pour le clavier standard américain, la touche /?
// VK_OEM_3 	0xC0 	Utilisé pour les caractères divers ; il peut varier par clavier. Pour le clavier standard américain, la touche `~
// 	0xC1-DA 	Réservé
// VK_OEM_4 	0xDB 	Utilisé pour les caractères divers ; il peut varier par clavier. Pour le clavier standard américain, la touche [{
// VK_OEM_5 	0xDC 	Utilisé pour les caractères divers ; il peut varier par clavier. Pour le clavier standard américain, la touche \\|
// VK_OEM_6 	0xDD 	Utilisé pour les caractères divers ; il peut varier par clavier. Pour le clavier standard américain, la touche ]}
// VK_OEM_7 	0xDE 	Utilisé pour les caractères divers ; il peut varier par clavier. Pour le clavier standard américain, la touche '"
// VK_OEM_8 	0xDF 	Utilisé pour les caractères divers ; il peut varier par clavier.
// 	0xE0 	Réservé
// 	0xE1 	Spécifique à l’OEM
// VK_OEM_102 	0xE2 	Touches <> sur le clavier standard américain, ou la touche \\| sur le clavier non us 102 touches
// 	0xE3-E4 	Spécifique à l’OEM
// VK_PROCESSKEY 	0xE5 	Clé DE PROCESSUS IME
// 	0xE6 	Spécifique à l’OEM
// VK_PACKET 	0xE7 	Utilisé pour passer des caractères Unicode comme s’ils étaient des séquences de touches. La touche VK_PACKET est le mot faible d’une valeur de clé virtuelle 32 bits utilisée pour les méthodes d’entrée non clavier. Pour plus d’informations, consultez Remarque dans KEYBDINPUT, SendInput, WM_KEYDOWNet WM_KEYUP
// 	0xE8 	Non attribué
// 	0xE9-F5 	Spécifique à l’OEM
// VK_ATTN 	0xF6 	Touche Attn
// VK_CRSEL 	0xF7 	Touche CrSel
// VK_EXSEL 	0xF8 	Touche ExSel
// VK_EREOF 	0xF9 	Effacer la clé EOF
// VK_PLAY 	0xFA 	Touche de lecture
// VK_ZOOM 	0xFB 	Touche de zoom
// VK_NONAME 	0xFC 	Réservé
// VK_PA1 	0xFD 	Clé PA1
// VK_OEM_CLEAR 	0xFE 	Effacer la clé