from lxml import etree
import sys
import os
import re
import tomllib

NS_XHTML = "http://www.w3.org/1999/xhtml"

def prettyprint(element):
  xml = etree.tostring(element, xml_declaration=True, encoding="utf-8", pretty_print=True, doctype="<!DOCTYPE HTML>")
  print(xml.decode(), end="")

def ns_html(eti):
  return "{%s}%s" % ( NS_XHTML, eti )

## Leer opciones
if len(sys.argv) < 2:
  raise Exception("Falta especificar una ruta de trabajo")

with open(sys.argv[1]+"/project.toml", "rb") as f:
    data = tomllib.load(f)

if "Navi" not in data:
  raise Exception("No existe tabla Navi en el documento")

# Generar opciones
h_doc = etree.Element(ns_html("html"), nsmap={ None: NS_XHTML })
h_head = etree.SubElement(h_doc, ns_html("head"))
if "name" not in data["Navi"]:
  raise Exception("Debes especificar el titulo de la aplicacion")
h_title = etree.SubElement(h_head, ns_html("title"))
h_title.text = data["Navi"]["name"]
etree.SubElement(h_head, ns_html("meta"), attrib={ "charset": "utf-8" })

if "theme_color" in data["Navi"]:
  etree.SubElement(h_head, ns_html("meta"), attrib={
    "name": "theme-color",
    "content": data["Navi"]["theme_color"]
  })

if "color_scheme":
  etree.SubElement(h_head, ns_html("meta"), attrib={
    "name": "color-scheme",
    "content": data["Navi"]["color_scheme"]
  })

# Reconocer objetos de texto
def check_discriminant(val):
  if False:
    raise Exception("No se reconoce el discriminador")
  return

os.chdir(os.path.join(sys.argv[1], "src"))
r_acts = re.compile(r"activities-?(.*)")
r_estilos = re.compile(r"styles-?(.*)")
r_vistas = re.compile(r"views-?(.*)")

for d in os.listdir():
  ## Actividades
  match = r_acts.match(d)
  if match:
    check_discriminant(match.groups()[0])
    for e in filter(re.compile(r".*\.js").match, os.listdir(path=d)):
      script = etree.SubElement(h_head, ns_html("script"), attrib={ "defer": "defer" })
      with open(os.path.join(d, e), "r") as f:
        script.text = etree.CDATA(f.read())
  else:
    ## Estilos
    match = r_estilos.match(d)
    if match:
      check_discriminant(match.groups()[0])
      for e in filter(re.compile(r".*\.css").match, os.listdir(path=d)):
        style = etree.SubElement(h_head, ns_html("style"))
        with open(os.path.join(d, e), "r") as f:
          style.text = etree.CDATA(f.read())
    else:
      ## Vistas
      match = r_vistas.match(d)
      if match:
        check_discriminant(match.groups()[0])
        for e in filter(re.compile(r".*\.xml").match, os.listdir(path=d)):
          view = etree.SubElement(h_head, ns_html("template"), attrib={
            "id": "view_"+e[:-4]
          })
          with open(os.path.join(d, e), "rb") as f:
            view.append(etree.parse(f).getroot())

etree.SubElement(h_doc, ns_html("body"))
prettyprint(h_doc)
