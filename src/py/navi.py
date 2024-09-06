from lxml import etree
import sys
import tomllib

NS_XHTML = "http://www.w3.org/1999/xhtml"

def prettyprint(element, **kwargs):
  xml = etree.tostring(element, pretty_print=True, **kwargs)
  print(xml.decode(), end='')

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

prettyprint(h_doc)
