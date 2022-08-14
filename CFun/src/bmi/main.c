#include <stdio.h>

int main(void) {

  float weight,height,bmi;

  printf("weight in pounds? ");
  scanf("%f",&weight);

  printf("height in inches? ");
  scanf("%f",&height);

  bmi=(weight/(height*height))*703;

  printf("bmi: %.2f\n",bmi);

  printf("result: ");

  if(bmi<18.5) printf("underweight");
  else if(bmi<24.9) printf("healthy");
  else if(bmi<29.9) printf("overweight");
  else printf("obese");

  printf("\n");

  return 0;
}
