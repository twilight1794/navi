from lxml import etree
import sys
import os
import re
import subprocess
import tomllib

NS_XHTML = "http://www.w3.org/1999/xhtml"

def prettyprint(element: etree.Element) -> None:
  xml = etree.tostring(element, xml_declaration=True, encoding="utf-8", pretty_print=True, doctype="<!DOCTYPE HTML>")
  print(xml.decode(), end="")

def ns_html(eti: str) -> str:
  return "{%s}%s" % ( NS_XHTML, eti )

def check_discriminant(val: str) -> str:
  r_disc_val = re.compile(r"^(xsm|sm|md|lg|xlg|xlg|port|land|night|day)(?:-(xsm|sm|md|lg|xlg|xlg|port|land|night|day))*$")
  r_disc_match = re.compile(r"(xsm|sm|md|lg|xlg|xlg|port|land|night|day)")
  if not r_disc_val.match(val):
    raise Exception("No se reconoce el discriminador")
  return r_disc_match.findall(val)

def get_name(name: str) -> str:
  if name.find(".") > -1:
    return name.rpartition(".")[2]
  return name

def get_css_media_query(discriminant: list) -> str:
  # Condiciones de tamaño de pantalla
  l_size = []
  if "xsm" in discriminant:
    l_size.append("(max-width: 639)")
  if "sm" in discriminant:
    l_size.append("(max-width: 767)")
  if "md" in discriminant:
    l_size.append("(max-width: 1023)")
  if "lg" in discriminant:
    l_size.append("(max-width: 1279)")
  if "xlg" in discriminant:
    l_size.append("(max-width: 1535)")
  if "xxlg" in discriminant:
    l_size.append("(min-width: 1536)")
  d_size = " or ".join(l_size)

  # Condiciones de orientación
  l_orientation = []
  if "port" in discriminant:
    l_orientation.append("(max-aspect-ratio: .9999)")
  if "land" in discriminant:
    l_orientation.append("(min-aspect-ratio: 1)")
  d_orientation = l_orientation[0] if len(l_orientation) == 1 else ""

  # Condiciones de tema
  l_theme = []
  if "day" in discriminant:
    l_theme.append("(prefers-color-scheme: light)")
  if "night" in discriminant:
    l_theme.append("(prefers-color-scheme: dark)")
  d_theme = l_theme[0] if len(l_theme) == 1 else ""

  return " and ".join(filter(None, (d_size, d_orientation, d_theme)))

## Leer opciones
if len(sys.argv) < 2:
  raise Exception("Falta especificar una ruta de trabajo")

with open(os.path.join(sys.argv[1], "project.toml"), "rb") as f:
    data = tomllib.load(f)

if "Navi" not in data:
  raise Exception("No existe tabla Navi en el documento")

# Generar opciones y estructura
h_doc = etree.Element(ns_html("html"), nsmap={ None: NS_XHTML })
h_head = etree.SubElement(h_doc, ns_html("head"))
if "name" not in data["Navi"]:
  raise Exception("Debes especificar el titulo de la aplicacion")
h_title = etree.SubElement(h_head, ns_html("title"))
h_title.text = data["Navi"]["name"]
etree.SubElement(h_head, ns_html("meta"), attrib={
  "name": "viewport",
  "content": "width=device-width, initial-scale=1.0"
})
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

# Archivos de Navi
os.chdir(os.path.join(os.path.realpath(__file__), "js"))
for e in filter(re.compile(r".*\.js").match, os.listdir()):
  script = etree.SubElement(h_head, ns_html("script"), attrib={ "defer": "defer" })
  with open(e, "r") as f:
    script.text = etree.CDATA(f.read())

os.chdir("../css")
for e in filter(re.compile(r".*\.css").match, os.listdir()):
  style = etree.SubElement(h_head, ns_html("style"))
  with open(e, "r") as f:
    style.text = etree.CDATA(f.read())

# Reconocer objetos de texto
os.chdir(os.path.join(sys.argv[1], "src"))
r_estilos = re.compile(r"styles-?(.*)")
r_vistas = re.compile(r"views-?(.*)")
r_imagenes = re.compile(r"images-?(.*)")
r_audios = re.compile(r"audios-?(.*)")
r_videos = re.compile(r"videos-?(.*)")

## Actividades0
for e in os.listdir(path="activities"):
  script = etree.SubElement(h_head, ns_html("script"), attrib={ "defer": "defer" })
  with open(os.path.join(d, e), "r") as f:
    script.text = etree.CDATA(f.read())

for d in os.listdir():
  ## Estilos
  match = r_estilos.match(d)
  if match:
    discriminants = check_discriminant(match.groups()[0])
    for e in filter(re.compile(r".*\.css").match, os.listdir(path=d)):
      style = etree.SubElement(h_head, ns_html("style"))
      with open(os.path.join(d, e), "r") as f:
        style_text = "@media (" + get_css_media_query(discriminants) + "){\n" + f.read() + "\n}"
        style.text = etree.CDATA(style_text)
    continue
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
    continue
  ## Imágenes
  match = r_imagenes.match(d)
  if match:
    check_discriminant(match.groups()[0])
    for e in filter(re.compile(r".*").match, os.listdir(path=d)):
      view = etree.SubElement(h_head, ns_html("link"), attrib={
        "rel": "navi:resource",
        "href": os.path.join(d, e),
        "type": subprocess.run(["file", "-bi", e], capture_output=True).stdout.decode().partition(";")[0],
        "media": match.groups()[0],
        "data-id": "@images/%s" % get_name(e)
      })
    continue

etree.SubElement(h_doc, ns_html("body"))
prettyprint(h_doc)
