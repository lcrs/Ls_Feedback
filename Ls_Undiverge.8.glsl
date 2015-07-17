// Undiverge a vector field
// Pass 8 - get gradient of pressure field and subtract from original
// lewis@lewissaunders.com

uniform sampler2D front, adsk_results_pass7;
uniform float adsk_result_w, adsk_result_h;
uniform float dpressure, amountmaster;

void main() {
	vec2 xy = gl_FragCoord.xy;
	vec2 px = vec2(1.0) / vec2(adsk_result_w, adsk_result_h);

	vec3 frontpix = texture2D(front, xy * px).rgb;
	float pressure = texture2D(adsk_results_pass7, xy * px).g;

	vec3 d = vec3(0.0);
	d.r -= texture2D(adsk_results_pass7, (xy + vec2(+dpressure, 0.0)) * px).g;
	d.r += texture2D(adsk_results_pass7, (xy + vec2(-dpressure, 0.0)) * px).g;
	d.g -= texture2D(adsk_results_pass7, (xy + vec2(0.0, +dpressure)) * px).g;
	d.g += texture2D(adsk_results_pass7, (xy + vec2(0.0, -dpressure)) * px).g;

	vec3 undiverged = frontpix + amountmaster * d;
	undiverged.b = frontpix.b;

	gl_FragColor = vec4(undiverged, pressure);
}
