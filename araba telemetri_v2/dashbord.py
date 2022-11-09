import cv2
img = cv2.imread("d1.png", cv2.IMREAD_COLOR)
#cv2.imshow("dashbord", img)
cv2.imshow('dashbord', cv2.resize(img,(1000,700),interpolation = cv2.INTER_CUBIC))
cv2.waitKey(0)
cv2.destroyAllWindows()
