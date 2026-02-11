//NGC MATCH

void CloseCrates(void) {
  if (crates != NULL) {
    NuLstDestroy(crates);
    crates = NULL;
  }
  return;
}
