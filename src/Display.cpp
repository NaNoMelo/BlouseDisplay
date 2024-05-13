#include "display.h"

Display::Display() {}

/**
 * TODO
 * fonctionnement des animations par sections en liste chainée
 * - plusieurs listes associées chacune à une animation
 * - les pixels sont assignés à une section et a un index dans la section en
 * elle même
 * - chaque frame, chaque index prends l'état du précédent de la section, et le
 * premier de la section prends un état défini par l'animation
 *
 */