#include <petscmat.h>
#include <stdio.h>
#include <fstream>

int main(int argc, char ** argv)
{
  PetscInitialize(&argc, &argv, NULL, NULL);

  PetscErrorCode err;
  err = PetscPrintf(PETSC_COMM_WORLD,"Initialized petsc\n");CHKERRQ(err);
  Mat A;
  err = MatCreate(PETSC_COMM_WORLD, &A); CHKERRQ(err);
  PetscInt m, n, i, j;
  PetscReal x;

  err = PetscPrintf(PETSC_COMM_WORLD,"Getting ready to read\n");CHKERRQ(err);
  
  std::ifstream f(argv[1]);
  f >> n >> m;
  err = MatSetSizes(A, PETSC_DECIDE, PETSC_DECIDE, n, m); CHKERRQ(err);
  err = MatSetUp(A); CHKERRQ(err);
  err = PetscPrintf(PETSC_COMM_WORLD,"Matrix size %ld x %ld\n", n, m);CHKERRQ(err);

  err = PetscPrintf(PETSC_COMM_WORLD,"Set up the matrix\n");CHKERRQ(err);

  while (!f.eof())
  {
    f >> i >> j >> x;
    err = MatSetValue(A, i-1, j-1, x, INSERT_VALUES); CHKERRQ(err);
  err = PetscPrintf(PETSC_COMM_WORLD,"%ld, %ld, %lf\n", i-1, j-1, x);CHKERRQ(err);
  }

  err = PetscPrintf(PETSC_COMM_WORLD,"Set all values\n");CHKERRQ(err);

  err = MatAssemblyBegin(A, MAT_FINAL_ASSEMBLY); CHKERRQ(err);
  err = MatAssemblyEnd(A, MAT_FINAL_ASSEMBLY); CHKERRQ(err);
  

  MatView(A, PETSC_VIEWER_STDOUT_WORLD);

  
  PetscViewer viewer;
  PetscViewerBinaryOpen(PETSC_COMM_WORLD, argv[2], FILE_MODE_WRITE, &viewer);
  MatView(A, viewer);

  PetscFinalize();

  return 0;
}
