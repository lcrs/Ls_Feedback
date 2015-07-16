// Undiverge a vector field
// Pass 7 - combine n output
// lewis@lewissaunders.com

uniform sampler2D front, adsk_results_pass2, adsk_results_pass4, adsk_results_pass6;
uniform float adsk_result_w, adsk_result_h;
uniform float d1, d2, d3, amount1, amount2, amount3, amountmaster;

void main() {
	vec2 xy = gl_FragCoord.xy;
	vec2 px = vec2(1.0) / vec2(adsk_result_w, adsk_result_h);

	vec3 frontpix = texture2D(front, xy * px).rgb;

	vec3 a = vec3(0.0);
	a.r += amount1 * texture2D(adsk_results_pass2, (xy + vec2(+d1, 0.0)) * px).r;
	a.r -= amount1 * texture2D(adsk_results_pass2, (xy + vec2(-d1, 0.0)) * px).r;
	a.g += amount1 * texture2D(adsk_results_pass2, (xy + vec2(0.0, +d1)) * px).g;
	a.g -= amount1 * texture2D(adsk_results_pass2, (xy + vec2(0.0, -d1)) * px).g;
	a.r += amount2 * texture2D(adsk_results_pass4, (xy + vec2(+d2, 0.0)) * px).r;
	a.r -= amount2 * texture2D(adsk_results_pass4, (xy + vec2(-d2, 0.0)) * px).r;
	a.g += amount2 * texture2D(adsk_results_pass4, (xy + vec2(0.0, +d2)) * px).g;
	a.g -= amount2 * texture2D(adsk_results_pass4, (xy + vec2(0.0, -d2)) * px).g;
	a.r += amount3 * texture2D(adsk_results_pass6, (xy + vec2(+d3, 0.0)) * px).r;
	a.r -= amount3 * texture2D(adsk_results_pass6, (xy + vec2(-d3, 0.0)) * px).r;
	a.g += amount3 * texture2D(adsk_results_pass6, (xy + vec2(0.0, +d3)) * px).g;
	a.g -= amount3 * texture2D(adsk_results_pass6, (xy + vec2(0.0, -d3)) * px).g;

	float pressure = a.r + a.g;
	vec2 divergence = vec2(dFdx(pressure), dFdy(pressure));

	vec2 undiverged = frontpix.rg + amountmaster * divergence;

	gl_FragColor = vec4(undiverged, frontpix.b, pressure);
}
