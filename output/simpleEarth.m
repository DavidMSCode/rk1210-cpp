load topo
% axesm ortho; framem
% meshm(topo,topolegend);demcmap(topo)
% view(3); daspectm('m',1)
% [latgrat,longrat] = meshgrat(topo,topolegend,[20 20]);
% stem3m(latgrat,longrat,500000*ones(size(latgrat)),'r')
% %figure
% axesm('globe','Geoid',earthRadius)
% meshm(topo,topolegend); demcmap(topo)
% view(3)
% %stem3m(latgrat,longrat,500000*ones(size(latgrat)),'r')
axesm('globe')
gridm('GLineStyle','-','Gcolor',[.8 .7 .6],'Galtitude', earthRadius()/1000)
load coast
plot3m(lat,long,earthRadius()/1000,'k')
view(3)
axis off; zoom(2)
set(gca,'Box','off')