#define rate_val 1.0f

void LoadCutSceneCharacters(struct csc_s *name) {
  
  while (name->path != NULL) {
    name->obj = InstNuGHGRead(&superbuffer_ptr,name->path);
    name++;
  }
  return;
}
