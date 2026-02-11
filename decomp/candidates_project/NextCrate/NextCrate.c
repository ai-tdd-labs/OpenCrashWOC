//NGC MATCH

struct crate_s * NextCrate(struct crate_s *a) {
  return (struct crate_s *)NuLstGetNext(crates,(struct nulnkhdr_s *)a);
}
